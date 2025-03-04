/*
 * ============= Assets Manager ================
 * Author: Sunil Sapkota
 * Description: Assets manager handles loading and managing of assets for usage across the engine
 */


#pragma once
#include "renderer/texture.h"
#include "scene/scene.h"
#include <filesystem>

namespace Everest {

    enum class AssetsType {
        NONE,
        TEXTURE,
        SHADER,
        SCENE,
        SCRIPT
    };


    class AssetsManager {
        public:

            static void init();
            static void quit();

            static AssetsType getAssetsType(std::filesystem::path path);
            static inline AssetsType getAssetsType(const char* path){
                return getAssetsType(std::filesystem::path(path));}

            static ref<Texture>& loadTexture(std::filesystem::path path);
            static ref<Scene>& loadScene(std::filesystem::path path);

            static ref<Texture>& loadTexture(const char* path){
                return loadTexture(std::filesystem::path(path));}
            static ref<Scene>& loadScene(const char* path){
                return loadScene(std::filesystem::path(path));}

            static inline const char* getScene_ext(){ return scene_ext;}
        private:
            std::unordered_map<std::filesystem::path, ref<Texture>> textures;
            std::unordered_map<std::filesystem::path, ref<Scene>> scenes;

        private:
            static AssetsManager* _instance;
            static const char* scene_ext;
            static const char* script_ext;
            static const char* shader_ext;
            static std::vector<std::string> texture_exts;

            AssetsManager() = default;
            ~AssetsManager();
    };
}
