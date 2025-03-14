/*
 * =============== Project Serializer ===================
 * Author: Sunil Sapkota
 * Description: Serialization and Deserialization definition for project-file
 */

#pragma once

#include "scene/def_components.h"
#include "scene/camera.h"
#include <filesystem>

namespace Everest {

    using scenepath_t = std::filesystem::path;

    // Everest project definition
    struct project_def_t {

        // project information
        std::string name;
        std::vector<scenepath_t> scene_sequence;
        
        // editor settings
        scenepath_t loaded_scene;
        vec4 background = vec4(0.1f, 0.1f, 0.1f, 1.f);

        transform_c editor_camera_transform;
        camera_c editor_camera_comp;

        static bool loadProject(const char* project_path, project_def_t& result);
        static void saveProject(const project_def_t& project, const char* project_path);
    };


}
