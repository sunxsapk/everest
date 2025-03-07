#include "scene/serializer.h"
#include "utils/assetsManager.h"

namespace YAML {
    template<>
    struct convert<Everest::vec2> {
        static Node encode(const Everest::vec2& rhs){
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, Everest::vec2& rhs){
            if(!node.IsSequence() || node.size() != 2) return false;
            rhs.x = node[0].as<f32>();
            rhs.y = node[1].as<f32>();
            return true;
        }
    };

    template<>
    struct convert<Everest::vec3> {
        static Node encode(const Everest::vec3& rhs){
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, Everest::vec3& rhs){
            if(!node.IsSequence() || node.size() != 3) return false;
            rhs.x = node[0].as<f32>();
            rhs.y = node[1].as<f32>();
            rhs.z = node[2].as<f32>();
            return true;
        }
    };

    template<>
    struct convert<Everest::vec4> {
        static Node encode(const Everest::vec4& rhs){
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, Everest::vec4& rhs){
            if(!node.IsSequence() || node.size() != 4) return false;
            rhs.x = node[0].as<f32>();
            rhs.y = node[1].as<f32>();
            rhs.z = node[2].as<f32>();
            rhs.w = node[3].as<f32>();
             return true;
        }
    };
}

namespace Everest {

    Scene* SceneSerializer::_scene = nullptr;

    void serializeEntity(Entity entity, YAML::Emitter& out){
        using namespace YAML;
        out << BeginMap;
        out << Key << "entity" << Value << (u64)entity.get<id_c>();

        if(entity.has<tag_c>()){
            out << entity.get<tag_c>();
        }

        if(entity.has<transform_c>()){
            out << entity.get<transform_c>();
        }

        if(entity.has<camera_c>()){
            out << entity.get<camera_c>();
        }

        if(entity.has<spriteRenderer_c>()){
            out << entity.get<spriteRenderer_c>();
        }

        if(entity.has<circleRenderer_c>()){
            out << entity.get<circleRenderer_c>();
        }

        if(entity.has<rigidbody2d_c>()){
            out << entity.get<rigidbody2d_c>();
        }

        if(entity.has<springJoint2d_c>()){
            out << entity.get<springJoint2d_c>();
        }

#ifndef __NO_3D__
        if(entity.has<rigidbody_c>()){
            out << entity.get<rigidbody_c>();
        }

        if(entity.has<springJoint_c>()){
            out << entity.get<springJoint_c>();
        }
#endif

        if(entity.has<boxCollider2d_c>()){
            out << entity.get<boxCollider2d_c>();
        }

        if(entity.has<circleCollider2d_c>()){
            out << entity.get<circleCollider2d_c>();
        }

        if(entity.has<EvScript>()){
            out << entity.get<EvScript>();
        }



        out << EndMap;
    }

    void SceneSerializer::serialize(const char* filepath){
        using namespace YAML;
        Emitter out;
        out << BeginMap;
        out << Key << "scene" << Value << _scene->_name.c_str();
        out << Key << "entities" << Value;

        out << BeginSeq;
        for(auto entity: _scene->_registry.view<tag_c>()){
            serializeEntity({entity, _scene}, out);
        }
        out << EndSeq;

        out << EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    bool SceneSerializer::deserialize(const char* filepath){
        using namespace YAML;

        Node data = YAML::LoadFile(filepath);
        if(!data["scene"]) return false;

        std::string scene_n = data["scene"].as<std::string>();
        EVLog_Msg("Deserializing scene %s", scene_n.c_str());
        _scene->_name = scene_n;

        auto entities = data["entities"];

        for(auto entity : entities){
            u64 uuid = entity["entity"].as<u64>();

            std::string name;
            auto tag = entity["tag_c"];
            if(tag) name = tag["tag"].as<std::string>();

            Entity n_ent = _scene->createEntityUUID(uuid, name.c_str());
        }

        for(auto entity : entities){
            u64 uuid = entity["entity"].as<u64>();
            Entity n_ent = _scene->getEntityFromId(uuid);

            {
                auto transform = entity["transform_c"];
                if(transform){
                    auto& tfr = n_ent.get<transform_c>();
                    tfr.position = transform["position"].as<vec3>();
                    tfr.rotation = transform["rotation"].as<vec3>();
                    tfr.scale = transform["scale"].as<vec3>();
                }
            }

            {
                auto camera = entity["camera_c"];
                if(camera){
                    auto& cam = n_ent.add<camera_c>();
                    cam.isPrimary = camera["isPrimary"]?camera["isPrimary"].as<bool>() : false;
                    cam.fixedAspect = camera["fixedAspect"].as<bool>();
                    cam.active = camera["active"]?camera["active"].as<bool>() : true;

                    if(camera["is2d"]){
                        if(camera["is2d"].as<bool>()) cam.set2d();
                        else cam.set3d();
                    }
                    if(camera["u_size_fov"]){
                        cam.set_lenssize(camera["u_size_fov"].as<f32>());
                    }
                    if(camera["aspect"]){
                        cam.set_aspect(camera["aspect"].as<f32>());
                    }
                    if(camera["near"]){
                        cam.set_near(camera["near"].as<f32>());
                    }
                    if(camera["far"]){
                        cam.set_far(camera["far"].as<f32>());
                    }
                }
            }

            {
                auto spriteRenderer = entity["spriteRenderer_c"];
                if(spriteRenderer){
                    auto txpath = spriteRenderer["texturePath"];
                    auto& comp = n_ent.add<spriteRenderer_c>();
                    comp.active = spriteRenderer["active"]?spriteRenderer["active"].as<bool>() : true;
                    comp.color = spriteRenderer["color"].as<vec4>();
                    comp.texture = txpath ? AssetsManager::loadTexture(txpath.as<std::string>().c_str()) : nullptr;
                    comp.startUV = spriteRenderer["startUV"].as<vec2>();
                    comp.sizeUV = spriteRenderer["sizeUV"].as<vec2>();
                }
            }

            {
                auto circleRenderer = entity["circleRenderer_c"];
                if(circleRenderer){
                    auto& comp = n_ent.add<circleRenderer_c>();
                    comp.active = circleRenderer["active"]?circleRenderer["active"].as<bool>() : true;
                    comp.color = circleRenderer["color"].as<vec4>();
                    comp.thickness = circleRenderer["thickness"].as<f32>();
                    comp.fade = circleRenderer["fade"].as<f32>();

                    auto txpath = circleRenderer["texturePath"];
                    comp.texture = txpath ? AssetsManager::loadTexture(txpath.as<std::string>().c_str()) : nullptr;
                    comp.startUV = circleRenderer["startUV"] ? circleRenderer["startUV"].as<vec2>() : vec2();
                    comp.sizeUV = circleRenderer["sizeUV"] ? circleRenderer["sizeUV"].as<vec2>() : vec2();
                }
            }

            {
                auto rigidbody2d = entity["rigidbody2d_c"];
                if(rigidbody2d){
                    auto& rb2d = n_ent.add<rigidbody2d_c>();
                    rb2d.active = rigidbody2d["active"]?rigidbody2d["active"].as<bool>() : true;
                    rb2d.velocity = rigidbody2d["velocity"].as<vec2>();
                    rb2d.angularVelocity = rigidbody2d["angularVelocity"].as<f32>();
                    rb2d.drag = rigidbody2d["drag"].as<f32>();
                    rb2d.definition = rigidbody2d["definition"].as<int>();
                    rb2d.inverseMass = rigidbody2d["inverseMass"].as<f32>();
                }
            }

            {
                auto springJoint2d = entity["springJoint2d_c"];
                if(springJoint2d){
                    auto& spr2d = n_ent.add<springJoint2d_c>();
                    spr2d.active = springJoint2d["active"]?springJoint2d["active"].as<bool>() : true;
                    u64 val = springJoint2d["anchor"].as<u64>();
                    spr2d.anchor = val ? _scene->getEntityFromId(val) : Entity();
                    spr2d.offset = springJoint2d["offset"].as<vec2>();
                    spr2d.springConstant = springJoint2d["springConstant"].as<f32>();
                    spr2d.damping = springJoint2d["damping"].as<f32>();
                    spr2d.restLength = springJoint2d["restLength"].as<f32>();
                }
            }

#ifndef __NO_3D__
            {
                auto rigidbody = entity["rigidbody_c"];
                if(rigidbody){
                    auto& rb = n_ent.add<rigidbody_c>();
                    rb.active = rigidbody["active"]?rigidbody["active"].as<bool>() : true;
                    rb.velocity = rigidbody["velocity"].as<vec3>();
                    rb.angularVelocity = rigidbody["angularVelocity"].as<vec3>();
                    rb.drag = rigidbody["drag"].as<f32>();
                    rb.useGravity = rigidbody["useGravity"].as<bool>();
                    rb.inverseMass = rigidbody["inverseMass"].as<f32>();
                }
            }

            {
                auto springJoint = entity["springJoint_c"];
                if(springJoint){
                    auto& spr = n_ent.add<springJoint_c>();
                    spr.active = springJoint["active"]?springJoint["active"].as<bool>() : true;
                    spr.anchor = springJoint["anchor"].as<vec3>();
                    spr.offset = springJoint["offset"].as<vec3>();
                    spr.springConstant = springJoint["springConstant"].as<f32>();
                    spr.damping = springJoint["damping"].as<f32>();
                    spr.restLength = springJoint["restLength"].as<f32>();
                }
            }
#endif

            {
                auto boxCollider2d = entity["boxCollider2d_c"];
                if(boxCollider2d){
                    auto& bc2d = n_ent.add<boxCollider2d_c>();
                    bc2d.active = boxCollider2d["active"]?boxCollider2d["active"].as<bool>() : true;
                    bc2d.box.offset = boxCollider2d["offset"].as<vec2>();
                    bc2d.box.halfExtents = boxCollider2d["halfExtents"].as<vec2>();
                    bc2d.restitution = boxCollider2d["restitution"].as<f32>();
                }
            }

            {
                auto circleCollider2d = entity["circleCollider2d_c"];
                if(circleCollider2d){
                    auto& cc2d = n_ent.add<circleCollider2d_c>();
                    cc2d.active = circleCollider2d["active"]?circleCollider2d["active"].as<bool>() : true;
                    cc2d.circle.offset = circleCollider2d["offset"].as<vec2>();
                    cc2d.circle.radius = circleCollider2d["radius"].as<f32>();
                    cc2d.restitution = circleCollider2d["restitution"].as<f32>();
                }
            }

            {
                using namespace Scripting;
                auto evscripts = entity["evscript_c"];
                if(evscripts){
                    EvScript& scr = n_ent.add<EvScript>();
                    for(auto script : evscripts){
                        std::string path = script["path"].as<std::string>();
                        scriptHandler_t& sh = scr.addScript(path);

                        auto ser = script["SERIALIZE"];
                        if(!ser.IsSequence()){
                            EVLog_Err("SERIALIZE must be a sequence");
                            continue;
                        }

                        luastate_t& stt = *sh.state;
                        for(const auto& entry : ser){
                            std::string key = entry["name"].as<std::string>();
                            Types t = (Types)entry["type"].as<u32>();

                            switch(t){
                                case Types::Int:
                                    stt[key] = entry["value"].as<i32>();
                                    break;
                                case Types::Bool:
                                    stt[key] = entry["value"].as<bool>();
                                    break;
                                case Types::Float:
                                    stt[key] = entry["value"].as<f64>();
                                    break;
                                case Types::String:
                                    stt[key] = entry["value"].as<std::string>();
                                    break;
                                case Types::Vec2:
                                    stt[key] = entry["value"].as<vec2>();
                                    break;
                                case Types::Vec3:
                                    stt[key] = entry["value"].as<vec3>();
                                    break;
                                case Types::Vec4:
                                case Types::Color:
                                    stt[key] = entry["value"].as<vec4>();
                                    break;
                                case Types::Entity:{
                                    Entity ent = _scene->getEntityFromId((UUID)entry["value"].as<u64>());
                                    stt[key] = ent;
                                    break;
                               }
                            }
                        }

                    }
                }
            }
        }

        return true;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const vec2& v){
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const vec3& v){
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const vec4& v){
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const transform_c& tfr){
        using namespace YAML;
        out << Key << "transform_c";
        out << BeginMap;
        
        out << Key << "position" << Value << tfr.position;
        out << Key << "rotation" << Value << tfr.rotation;
        out << Key << "scale" << Value << tfr.scale;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const tag_c& tag){
        using namespace YAML;
        out << Key << "tag_c";
        out << BeginMap;

        out << Key << "tag" << Value << tag.tag;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const camera_c& camera){
            using namespace YAML;
        out << Key << "camera_c";
        out << BeginMap;

        out << Key << "active" << Value << camera.active;
        out << Key << "fixedAspect" << Value << camera.fixedAspect;
        out << Key << "isPrimary" << Value << camera.isPrimary;
        out << Key << "is2d" << Value << camera.is2d();

        out << Key << "u_size_fov" << Value << camera.get_lenssize();
        out << Key << "aspect" << Value << camera.get_aspect();
        out << Key << "near" << Value << camera.get_near();
        out << Key << "far" << Value << camera.get_far();

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const spriteRenderer_c& spriteRenderer){
        using namespace YAML;
        out << Key << "spriteRenderer_c";
        out << BeginMap;

        out << Key << "active" << Value << spriteRenderer.active;
        out << Key << "color" << Value << spriteRenderer.color;
        out << Key << "startUV" << Value << spriteRenderer.startUV;
        out << Key << "sizeUV" << Value << spriteRenderer.sizeUV;

        const ref<Texture>& tx = spriteRenderer.texture;
        if(tx != nullptr) out << Key << "texturePath" << Value << tx->getPath();

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const circleRenderer_c& circleRenderer){
        using namespace YAML;
        out << Key << "circleRenderer_c";
        out << BeginMap;

        out << Key << "active" << Value << circleRenderer.active;
        out << Key << "color" << Value << circleRenderer.color;
        out << Key << "startUV" << Value << circleRenderer.startUV;
        out << Key << "sizeUV" << Value << circleRenderer.sizeUV;
        out << Key << "thickness" << Value << circleRenderer.thickness;
        out << Key << "fade" << Value << circleRenderer.fade;

        const ref<Texture>& tx = circleRenderer.texture;
        if(tx != nullptr) out << Key << "texturePath" << Value << tx->getPath();

        out << EndMap;
        return out;
    }


    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody2d_c& rb2d){
        using namespace YAML;
        out << Key << "rigidbody2d_c";
        out << BeginMap;

        out << Key << "active" << Value << rb2d.active;
        out << Key << "inverseMass" << Value << rb2d.inverseMass;
        out << Key << "drag" << Value << rb2d.drag;
        out << Key << "velocity" << Value << rb2d.velocity;
        out << Key << "angularVelocity" << Value << rb2d.angularVelocity;
        out << Key << "definition" << Value << rb2d.definition;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, springJoint2d_c& spr){
        using namespace YAML;
        out << Key << "springJoint2d_c";
        out << BeginMap;

        out << Key << "active" << Value << spr.active;
        out << Key << "anchor" << Value << (spr.anchor.isValid() ? (u64) spr.anchor.get<Everest::id_c>().id : 0);
        out << Key << "offset" << Value << spr.offset;
        out << Key << "springConstant" << Value << spr.springConstant;
        out << Key << "damping" << Value << spr.damping;
        out << Key << "restLength" << Value << spr.restLength;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const boxCollider2d_c& boxCollider2d){
        using namespace YAML;
        out << Key << "boxCollider2d_c";
        out << BeginMap;

        out << Key << "active" << Value << boxCollider2d.active;
        out << Key << "offset" << Value << boxCollider2d.box.offset;
        out << Key << "halfExtents" << Value << boxCollider2d.box.halfExtents;
        out << Key << "restitution" << Value << boxCollider2d.restitution;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const circleCollider2d_c& circleCollider2d){
        using namespace YAML;
        out << Key << "circleCollider2d_c";
        out << BeginMap;

        out << Key << "active" << Value << circleCollider2d.active;
        out << Key << "offset" << Value << circleCollider2d.circle.offset;
        out << Key << "radius" << Value << circleCollider2d.circle.radius;
        out << Key << "restitution" << Value << circleCollider2d.restitution;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const EvScript& script){ // TODO: active
        using namespace YAML;
        using namespace Scripting;
        if(!script.scripts.size()) return out;

        out << Key << "evscript_c";
        out << BeginSeq;
        for(auto& sh : script.scripts){
            out << BeginMap;
            out << Key << "path" << Value << sh.scriptpath.c_str();

            luastate_t& st = *sh.state;
            sol::table ser;
            if(sh.getSerializedFields(ser)) {
                out << Key << "SERIALIZE";
                out << BeginSeq;
                for(auto& [key, type] : ser){
                    if(!key.valid() || !type.valid()) continue;
                    auto val = st[key.as<std::string>()];
                    if(!val.valid()) continue;

                    out << BeginMap;
                    out << Key << "name" << Value << key.as<std::string>();
                    out << Key << "type" << Value << (u32)type.as<Types>();
                    out << Key << "value" << Value;

                    switch(type.as<Types>()){
                        case Types::Int:
                            out << (i32)val;
                            break;
                        case Types::Bool:
                            out << (bool)val;
                            break;
                        case Types::Float:
                            out << (f64)val;
                            break;
                        case Types::String:
                            out << (std::string)val;
                            break;
                        case Types::Vec2:
                            out << (vec2&)val;
                            break;
                        case Types::Vec3:
                            out << (vec3&)val;
                            break;
                        case Types::Vec4:
                        case Types::Color:
                            out << (vec4&)val;
                            break;
                        case Types::Entity:
                            out << (u64)((Entity&)val).get<id_c>().id;
                            break;
                    }
                    out << EndMap;
                }
                out << EndSeq;
            }

            out << EndMap;
        }
        out << EndSeq;
        return out;
    }

#ifndef __NO_3D__
    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody_c& rb){
        using namespace YAML;
        out << Key << "rigidbody_c";
        out << BeginMap;

        out << Key << "active" << Value << rb.active;
        out << Key << "inverseMass" << Value << rb.inverseMass;
        out << Key << "drag" << Value << rb.drag;
        out << Key << "velocity" << Value << rb.velocity;
        out << Key << "angularVelocity" << Value << rb.angularVelocity;
        out << Key << "useGravity" << Value << rb.useGravity;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const springJoint_c& spr){
        using namespace YAML;
        out << Key << "springJoint_c";
        out << BeginMap;

        out << Key << "active" << Value << spr.active;
        out << Key << "anchor" << Value << spr.anchor;
        out << Key << "offset" << Value << spr.offset;
        out << Key << "springConstant" << Value << spr.springConstant;
        out << Key << "damping" << Value << spr.damping;
        out << Key << "restLength" << Value << spr.restLength;

        out << EndMap;
        return out;
    }
#endif
}
