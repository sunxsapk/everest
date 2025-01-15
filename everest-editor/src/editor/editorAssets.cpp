#include "editorAssets.h"

namespace Everest {

    EditorAssets* EditorAssets::_instance = nullptr;

    void EditorAssets::init(){
        ASSERT(_instance == nullptr, "Editor Assest handler already initialized");
        _instance = new EditorAssets;
    }

    void EditorAssets::quit(){
        delete _instance;
    }

    spriteRenderer_c& EditorAssets::getIcon(IconType icon){
        ASSERT(_instance != nullptr, "Uninitialized assest handler");
        ASSERT((int)icon < _instance->icons.size(), "Uninitialized icons");
        return _instance->icons[(int)icon];
    }
    
    EditorAssets::EditorAssets(){
        ref<Texture> icontex = AssetsManager::loadTexture("assets/sprites/icons.png");
        SpriteSheet sheet(icontex, {128, 128});
        icons.push_back(sheet.getSprite({0,7}, {1,1}));
        icons.push_back(sheet.getSprite({1,7}, {1,1}));
        icons.push_back(sheet.getSprite({2,7}, {1,1}));
        icons.push_back(sheet.getSprite({3,7}, {1,1}));
        icons.push_back(sheet.getSprite({4,7}, {1,1}));
    }
}
