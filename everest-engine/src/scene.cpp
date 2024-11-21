#include "scene/scene.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "renderer/renderer.h"
#include "renderer/renderer2d.h"

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
        auto sprgrp = _registry.group<transform_c>(entt::get<spriteRenderer_c>);
        for(auto ent : sprgrp){
            auto [tfr, spr] = sprgrp.get(ent);
            Renderer2D::drawSprite(tfr, spr.sprite, spr.color);
        }
    }
    void Scene::onViewportResize(uvec2 viewportSize){
        _viewportSize = viewportSize;
        f32 aspect = (float)viewportSize.x / viewportSize.y;

        auto cams = _registry.view<camera_c>();
        for(auto ent : cams){
            auto& cam = cams->get(ent);
            if(!cam.fixedAspect){
                cam.camera.setAspect(aspect);
            }
        }
    }
}
