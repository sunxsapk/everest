#include "scene/scene.h"
#include "scene/components.h"


namespace Everest {

    Scene::Scene(){
    }

    Entity Scene::createEntity(const char* name){
        Entity _en = {_registry.create(), this};
        _en.add<transform_c>();
        _en.add<tag_c>(name);
        return _en;
    }

    Scene::~Scene(){
    }

    void Scene::onUpdate(){
    }
}
