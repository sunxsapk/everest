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

        if(entity.has<rigidbody_c>()){
            out << entity.get<rigidbody_c>();
        }

        if(entity.has<springJoint2d_c>()){
            out << entity.get<springJoint2d_c>();
        }

        if(entity.has<springJoint_c>()){
            out << entity.get<springJoint_c>();
        }

        if(entity.has<boxCollider2d_c>()){
            out << entity.get<boxCollider2d_c>();
        }

        if(entity.has<circleCollider2d_c>()){
            out << entity.get<circleCollider2d_c>();
        }



        out << EndMap;
    }

    void SceneSerializer::serialize(const char* filepath){
        using namespace YAML;
        Emitter out;
        out << BeginMap;
        out << Key << "scene" << Value << _scene->_name.c_str();
        out << Key << "entities" << Value << BeginSeq;

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
                    auto& cam = n_ent.add<camera_c>(camera_c{
                        });
                    cam.isPrimary = camera["isPrimary"]?camera["isPrimary"].as<bool>() : false;
                    cam.fixedAspect = camera["fixedAspect"].as<bool>();
                    
                    auto ortho = camera["orthographic_d"];
                    OrthographicData odat{
                        .orthoSize = ortho["size"].as<f32>(),
                        .aspect = ortho["aspect"].as<f32>(),
                        .near = ortho["near"].as<f32>(),
                        .far = ortho["far"].as<f32>(),
                    };
                    cam.setOrthographicData(odat);

                    auto persp = camera["perspective_d"];
                    PerspectiveData pdat{
                        .fov = persp["fov"].as<f32>(),
                        .aspect = persp["aspect"].as<f32>(),
                        .near = persp["near"].as<f32>(),
                        .far = persp["far"].as<f32>(),
                    };
                    cam.setPerspectiveData(pdat);

                    cam.setType((CameraType)camera["type"].as<u32>());
                }
            }

            {
                auto spriteRenderer = entity["spriteRenderer_c"];
                if(spriteRenderer){
                    auto txpath = spriteRenderer["texturePath"];
                    n_ent.add<spriteRenderer_c>(spriteRenderer_c{
                            .texture = txpath ?
                                AssetsManager::loadTexture(txpath.as<std::string>().c_str()) :
                                nullptr,
                            .color = spriteRenderer["color"].as<vec4>(),
                            .startUV = spriteRenderer["startUV"].as<vec2>(),
                            .sizeUV = spriteRenderer["sizeUV"].as<vec2>(),
                        });
                }
            }

            {
                auto circleRenderer = entity["circleRenderer_c"];
                if(circleRenderer){
                    n_ent.add<circleRenderer_c>(circleRenderer_c{
                            .color = circleRenderer["color"].as<vec4>(),
                            .thickness = circleRenderer["thickness"].as<f32>(),
                            .fade = circleRenderer["fade"].as<f32>(),
                        });
                }
            }

            {
                auto rigidbody2d = entity["rigidbody2d_c"];
                if(rigidbody2d){
                    auto& rb2d = n_ent.add<rigidbody2d_c>();
                    rb2d.velocity = rigidbody2d["velocity"].as<vec2>();
                    rb2d.drag = rigidbody2d["drag"].as<f32>();
                    rb2d.useGravity = rigidbody2d["useGravity"].as<bool>();
                    rb2d.inverseMass = rigidbody2d["inverseMass"].as<f32>();
                }
            }

            {
                auto rigidbody = entity["rigidbody_c"];
                if(rigidbody){
                    auto& rb = n_ent.add<rigidbody_c>();
                    rb.velocity = rigidbody["velocity"].as<vec3>();
                    rb.drag = rigidbody["drag"].as<f32>();
                    rb.useGravity = rigidbody["useGravity"].as<bool>();
                    rb.inverseMass = rigidbody["inverseMass"].as<f32>();
                }
            }

            {
                auto springJoint2d = entity["springJoint2d_c"];
                if(springJoint2d){
                    auto& spr2d = n_ent.add<springJoint2d_c>();
                    spr2d.anchor = springJoint2d["anchor"].as<vec2>();
                    spr2d.offset = springJoint2d["offset"].as<vec2>();
                    spr2d.springConstant = springJoint2d["springConstant"].as<f32>();
                    spr2d.damping = springJoint2d["damping"].as<f32>();
                    spr2d.restLength = springJoint2d["restLength"].as<f32>();
                }
            }

            {
                auto springJoint = entity["springJoint_c"];
                if(springJoint){
                    auto& spr = n_ent.add<springJoint_c>();
                    spr.anchor = springJoint["anchor"].as<vec3>();
                    spr.offset = springJoint["offset"].as<vec3>();
                    spr.springConstant = springJoint["springConstant"].as<f32>();
                    spr.damping = springJoint["damping"].as<f32>();
                    spr.restLength = springJoint["restLength"].as<f32>();
                }
            }

            {
                auto boxCollider2d = entity["boxCollider2d_c"];
                if(boxCollider2d){
                    auto& bc2d = n_ent.add<boxCollider2d_c>();
                    bc2d.box.offset = boxCollider2d["offset"].as<vec2>();
                    bc2d.box.halfExtents = boxCollider2d["halfExtents"].as<vec2>();
                    bc2d.restitution = boxCollider2d["restitution"].as<f32>();
                }
            }

            {
                auto circleCollider2d = entity["circleCollider2d_c"];
                if(circleCollider2d){
                    auto& cc2d = n_ent.add<circleCollider2d_c>();
                    cc2d.circle.offset = circleCollider2d["offset"].as<vec2>();
                    cc2d.circle.radius = circleCollider2d["radius"].as<f32>();
                    cc2d.restitution = circleCollider2d["restitution"].as<f32>();
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

        out << Key << "fixedAspect" << Value << camera.fixedAspect;
        out << Key << "isPrimary" << Value << camera.isPrimary;
        out << Key << "type" << Value << (u32)camera.getType();

        out << Key << "perspective_d";
        out << BeginMap;
        out << Key << "fov" << Value << camera.getPersp_fov();
        out << Key << "aspect" << Value << camera.getPersp_aspect();
        out << Key << "near" << Value << camera.getPersp_near();
        out << Key << "far" << Value << camera.getPersp_far();
        out << EndMap;

        out << Key << "orthographic_d";
        out << BeginMap;
        out << Key << "size" << Value << camera.getOrtho_size();
        out << Key << "aspect" << Value << camera.getOrtho_aspect();
        out << Key << "near" << Value << camera.getOrtho_near();
        out << Key << "far" << Value << camera.getOrtho_far();
        out << EndMap;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const spriteRenderer_c& spriteRenderer){
        using namespace YAML;
        out << Key << "spriteRenderer_c";
        out << BeginMap;

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

        out << Key << "color" << Value << circleRenderer.color;
        out << Key << "thickness" << Value << circleRenderer.thickness;
        out << Key << "fade" << Value << circleRenderer.fade;

        out << EndMap;
        return out;
    }


    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody2d_c& rb2d){
        using namespace YAML;
        out << Key << "rigidbody2d_c";
        out << BeginMap;

        out << Key << "inverseMass" << Value << rb2d.inverseMass;
        out << Key << "drag" << Value << rb2d.drag;
        out << Key << "velocity" << Value << rb2d.velocity;
        out << Key << "useGravity" << Value << rb2d.useGravity;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody_c& rb){
        using namespace YAML;
        out << Key << "rigidbody_c";
        out << BeginMap;

        out << Key << "inverseMass" << Value << rb.inverseMass;
        out << Key << "drag" << Value << rb.drag;
        out << Key << "velocity" << Value << rb.velocity;
        out << Key << "useGravity" << Value << rb.useGravity;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const springJoint2d_c& spr){
        using namespace YAML;
        out << Key << "springJoint2d_c";
        out << BeginMap;

        out << Key << "anchor" << Value << spr.anchor;
        out << Key << "offset" << Value << spr.offset;
        out << Key << "springConstant" << Value << spr.springConstant;
        out << Key << "damping" << Value << spr.damping;
        out << Key << "restLength" << Value << spr.restLength;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const springJoint_c& spr){
        using namespace YAML;
        out << Key << "springJoint_c";
        out << BeginMap;

        out << Key << "anchor" << Value << spr.anchor;
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

        out << Key << "offset" << Value << circleCollider2d.circle.offset;
        out << Key << "radius" << Value << circleCollider2d.circle.radius;
        out << Key << "restitution" << Value << circleCollider2d.restitution;

        out << EndMap;
        return out;
    }
}
