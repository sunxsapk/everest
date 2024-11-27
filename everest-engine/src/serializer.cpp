#include "scene/serializer.h"

namespace Everest {

    Serializer::Serializer(const ref<Scene>& scene)
    :_scene(scene){
    }

    void serializeEntity(Entity entity, YAML::Emitter& out){
        using namespace YAML;
        out << BeginMap;
        out << Key << "Entity" << Value << (u32)entity;

        if(entity.has<tag_c>()){
            out << entity.get<tag_c>();
        }

        if(entity.has<transform_c>()){
            out << entity.get<transform_c>();
        }

        out << EndMap;
    }

    void Serializer::serialize(const char* filepath){
        using namespace YAML;
        Emitter out;
        out << BeginMap;
        out << Key << "Scene" << Value << "Untitled";
        out << Key << "Entities" << Value << BeginSeq;

        for(auto entity: _scene->_registry.view<tag_c>()){
            serializeEntity({entity, _scene.get()}, out);
        }

        out << EndSeq;
        out << EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    bool Serializer::deserialize(const char* filepath){
        ASSERT(false, "Not implemented yet");
        return false;
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

}
