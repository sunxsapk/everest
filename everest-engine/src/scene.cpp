#include "scene/scene.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "renderer/renderer.h"
#include "renderer/renderer2d.h"

namespace Everest {

    Scene::Scene(){
        EV_profile_function();

    }

    Entity Scene::createEntity(const char* name){
        EV_profile_function();



        Entity _en = {_registry.create(), this};
        _en.add<transform_c>();
        _en.add<tag_c>(name);
        return _en;
    }

    Scene::~Scene(){
        EV_profile_function();



        _registry.view<nativeScript_c>().each([=](auto ent, nativeScript_c nscript){
                if(nscript._instance){
                    nscript._instance->onDestroy();
                    nscript.destroy(&nscript);
                }
            });
    }

    void Scene::onUpdate(){
        EV_profile_function();


        _registry.view<nativeScript_c>().each([=](auto ent, nativeScript_c& nscript){
                if(!nscript._instance){
                    nscript._instance = nscript.create();
                    nscript._instance->_entity = {ent, this};
                    nscript._instance->onCreate();
                }
                nscript._instance->onUpdate();
            });


        {
            auto sprgrp = _registry.group<transform_c>(entt::get<spriteRenderer_c>);
            for(auto ent : sprgrp){
                auto [tfr, spr] = sprgrp.get(ent);
                Renderer2D::drawSprite(tfr, spr.sprite, spr.color);
            }
        }
    }

    void Scene::onViewportResize(uvec2 viewportSize){
        EV_profile_function();


        _viewportSize = viewportSize;
        f32 aspect = (float)viewportSize.x / viewportSize.y;

        auto cams = _registry.view<camera_c>();
        for(auto ent : cams){
            auto& cam = cams->get(ent);
            if(!cam.fixedAspect){
                if(cam.camera.getType() == Orthographic) cam.camera.setOrtho_aspect(aspect);
                else cam.camera.setPersp_aspect(aspect);
            }
        }
    }
}
