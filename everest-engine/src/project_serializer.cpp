#include "core/project_serializer.h"
#include "scene/yaml_extensions.h"

namespace Everest {

    bool project_def_t::loadProject(const char* project_path, project_def_t& result){
        using namespace YAML;

        if(!project_path) return false;

        Node data = YAML::LoadFile(project_path);
        if(!data["name"]) return false;

        result.name = data["name"].as<std::string>();
        EVLog_Msg("Loading Project %s from %s", result.name.c_str(), project_path);

        auto scenes = data["scene_sequence"];
        if(scenes){
            for(auto scene : scenes){
                result.scene_sequence.push_back(std::filesystem::path(scene.as<std::string>()));
            }
        }

        if(data["loaded_scene"]){
            result.loaded_scene = std::filesystem::path(data["loaded_scene"].as<std::string>());
        }

        if(data["background"]){
            result.background = data["background"].as<vec4>();
        }

        {
            auto transform = data["transform_c"];
            if(transform){
                result.editor_camera_transform.position = transform["position"].as<vec3>();
                result.editor_camera_transform.rotation = transform["rotation"].as<vec3>();
                result.editor_camera_transform.scale = transform["scale"].as<vec3>();
            }
        }

        {
            auto camera = data["camera_c"];
            if(camera){
                auto& cam = result.editor_camera_comp;
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


        return true;
    }

    void project_def_t::saveProject(const project_def_t& project, const char* project_path){

        using namespace YAML;

        Emitter out;
        out << BeginMap;
        out << Key << "name" << Value << project.name.c_str();

        out << Key << "scene_sequence" << Value;
        out << BeginSeq;
        for(auto& scene : project.scene_sequence){
            out << scene.string().c_str();
        }
        out << EndSeq;

        out << Key << "loaded_scene" << Value << project.loaded_scene.string().c_str();
        out << Key << "background" << Value << project.background;
        out << project.editor_camera_transform;
        out << project.editor_camera_comp;

        out << EndMap;

        std::ofstream fout(project_path);
        fout << out.c_str();
    }

}
