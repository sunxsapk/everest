#include "scene/serializer.h"
#include "scene/yaml_extensions.h"
#include "utils/assetsManager.h"


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

/*#ifndef __NO_3D__
        if(entity.has<rigidbody_c>()){
            out << entity.get<rigidbody_c>();
        }

        if(entity.has<springJoint_c>()){
            out << entity.get<springJoint_c>();
        }
#endif*/

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
                    comp.sizeUV = circleRenderer["sizeUV"] ? circleRenderer["sizeUV"].as<vec2>() : vec2(1.f);
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

/*#ifndef __NO_3D__
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
#endif*/

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
                        if(!std::filesystem::exists(path)) continue;

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
}
