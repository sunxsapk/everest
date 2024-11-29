#include "scene/serializer.h"

namespace YAML {
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
        out << Key << "entity" << Value << (u64)entity;

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

        out << EndMap;
    }

    void SceneSerializer::serialize(const char* filepath){
        using namespace YAML;
        Emitter out;
        out << BeginMap;
        out << Key << "scene" << Value << "Untitled";
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

        /*std::ifstream stream(filepath);
        std::stringstream sstream;
        sstream << stream.rdbuf();*/

        Node data = YAML::LoadFile(filepath);
        if(!data["scene"]) return false;

        std::string scene_n = data["scene"].as<std::string>();
        EVLog_Msg("Deserializing scene %s", scene_n.c_str());

        auto entities = data["entities"];

        for(auto entity : entities){
            u64 uuid = entity["entity"].as<u64>();

            std::string name;
            auto tag = entity["tag_c"];
            if(tag) name = tag["tag"].as<std::string>();

            Entity n_ent = _scene->createEntity(name.c_str());

            auto transform = entity["transform_c"];
            if(transform){
                auto& tfr = n_ent.get<transform_c>();
                tfr.position = transform["position"].as<vec3>();
                tfr.rotation = transform["rotation"].as<vec3>();
                tfr.scale = transform["scale"].as<vec3>();
            }

            auto camera = entity["camera_c"];
            if(camera){
                auto& cam = n_ent.add<camera_c>(camera_c{
                        .fixedAspect = camera["fixedAspect"].as<bool>()
                    });
                
                auto ortho = camera["orthographic_d"];
                OrthographicData odat{
                    .orthoSize = ortho["size"].as<f32>(),
                    .aspect = ortho["aspect"].as<f32>(),
                    .near = ortho["near"].as<f32>(),
                    .far = ortho["far"].as<f32>(),
                };
                cam.camera.setOrthographicData(odat);

                auto persp = camera["perspective_d"];
                PerspectiveData pdat{
                    .fov = persp["fov"].as<f32>(),
                    .aspect = persp["aspect"].as<f32>(),
                    .near = persp["near"].as<f32>(),
                    .far = persp["far"].as<f32>(),
                };
                cam.camera.setPerspectiveData(pdat);

                cam.camera.setType((CameraType)camera["type"].as<u32>());
            }

            auto spriteRenderer = entity["spriteRenderer_c"];
            if(spriteRenderer){
                auto& spr = n_ent.add<spriteRenderer_c>(spriteRenderer_c{
                        .color = spriteRenderer["color"].as<vec4>()
                    });
            }
        }

        return true;
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
        out << Key << "type" << Value << (u32)camera.camera.getType();

        out << Key << "perspective_d";
        out << BeginMap;
        out << Key << "fov" << Value << camera.camera.getPersp_fov();
        out << Key << "aspect" << Value << camera.camera.getPersp_aspect();
        out << Key << "near" << Value << camera.camera.getPersp_near();
        out << Key << "far" << Value << camera.camera.getPersp_far();
        out << EndMap;

        out << Key << "orthographic_d";
        out << BeginMap;
        out << Key << "size" << Value << camera.camera.getOrtho_size();
        out << Key << "aspect" << Value << camera.camera.getOrtho_aspect();
        out << Key << "near" << Value << camera.camera.getOrtho_near();
        out << Key << "far" << Value << camera.camera.getOrtho_far();
        out << EndMap;

        out << EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const spriteRenderer_c& spriteRenderer){
        using namespace YAML;
        out << Key << "spriteRenderer_c";
        out << BeginMap;

        out << Key << "color" << Value << spriteRenderer.color;

        out << EndMap;
        return out;
    }
}
