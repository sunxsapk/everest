#include "utils/assetsManager.h"
#include "scene/serializer.h"
#include "scene/scenemanager.h"


namespace Everest {

    AssetsManager* AssetsManager::_instance = nullptr;

    const char* AssetsManager::scene_ext = ".everest";
    const char* AssetsManager::shader_ext = ".glsl";
    const char* AssetsManager::script_ext = ".lua";
    std::vector<std::string> AssetsManager::texture_exts {".png", ".jpg", ".jpeg"};


    void AssetsManager::init(){
        _instance = new AssetsManager();
    }

    void AssetsManager::quit(){
        delete _instance;
    }

    AssetsType AssetsManager::getAssetsType(std::filesystem::path path){
        if(path.extension() == scene_ext) return AssetsType::SCENE;
        else if(path.extension() == shader_ext) return AssetsType::SHADER;
        else if(path.extension() == script_ext) return AssetsType::SCRIPT;
        else if( std::find(texture_exts.begin(), texture_exts.end(), path.extension())
                != texture_exts.end()) return AssetsType::TEXTURE;
        else return AssetsType::NONE;
    }

    ref<Texture>& AssetsManager::loadTexture(std::filesystem::path path){
        if(_instance->textures.find(path) != _instance->textures.end()) return _instance->textures[path];
        _instance->textures[path] = createRef<Texture>(path.c_str());
        return _instance->textures[path];
    }

    ref<Scene>& AssetsManager::loadScene(std::filesystem::path path){
        if(_instance->scenes.find(path) != _instance->scenes.end()){
            SceneManager::setSceneTarget(path);
            return _instance->scenes[path];
        }
        ref<Scene> scene = createRef<Scene>();
        SceneSerializer::setSerializationContext(scene.get());
        bool _success = SceneSerializer::deserialize(path.c_str());
        ASSERT(_success, "Unable to load scene"); // TODO: make an exception instead
        _instance->scenes[path] = scene;
        SceneManager::setSceneTarget(path);
        return _instance->scenes[path];
    }

    AssetsManager::~AssetsManager(){
        textures.clear();
        scenes.clear();
    }
}
