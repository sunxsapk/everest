#include "scene/scene.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "renderer/renderer2d.h"
#include "math/utils.h"
#include "physics/physicsworld.h"
#include "core/time.h"

namespace Everest {

    Scene::Scene(const char* name): _name(name){
        EV_profile_function();
    }

    Scene::Scene(std::string name): _name(name){
        EV_profile_function();
    }

    Entity Scene::createEntity(const char* name){
        EV_profile_function();

        return createEntityUUID(UUID(), name);
    }


    Entity Scene::createEntityUUID(UUID id, const char * name){
        EV_profile_function();



        Entity _en {_registry.create(), this};
        _en.add<transform_c>();
        _en.add<id_c>(id);
        _en.add<tag_c>(name);
        return _en;
    }

    void Scene::destroyEntity(Entity& entity){
        EV_profile_function();
        
        _registry.destroy(entity);
        entity._id = entt::null;
    } 

    Scene::~Scene(){
        EV_profile_function();

        _registry.view<nativeScript_c>().each([=](auto ent, nativeScript_c nscript){
                if(nscript._instance){
                    nscript._instance->onDestroy();
                    nscript.destroy(&nscript);
                }
            });
        _registry.clear();
    }

    void Scene::fetchTargetCamera(){
        EV_profile_function();

        auto camgrp = _registry.group<camera_c>(entt::get<transform_c>);
        for (auto ent: camgrp){
            const auto& [cam, tfr] = camgrp.get(ent);
            if(cam.isPrimary){
                mainCamera.camera = &cam;
                mainCamera.transform = &tfr;
                break;
            }
        }

    }

    void Scene::onRender(){ 
        EV_profile_function();

        if(mainCamera.camera == nullptr) fetchTargetCamera();
        if(mainCamera.camera == nullptr) return;

        Renderer2D::beginScene(*mainCamera.camera, *mainCamera.transform);

        auto sprgrp = _registry.group<spriteRenderer_c>(entt::get<transform_c>);
        for(auto ent : sprgrp){
            const auto& [spr, tfr] = sprgrp.get(ent);
            Renderer2D::drawSprite(tfr, spr
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
        }

        auto cirgrp = _registry.group<circleRenderer_c>(entt::get<transform_c>);
        for(auto ent : cirgrp){
            const auto& [cir, tfr] = cirgrp.get(ent);
            Renderer2D::drawCircle(tfr, cir.color, cir.thickness, cir.fade
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
        }

        Renderer2D::endScene();

    }

    void drawCameraGizmo(transform_c& tfr, camera_c& cam, u32 id){
        if(cam.getType() == CameraType::Perspective){
            vec3 scale(cam.getPersp_aspect(), 1.f, 1.f);
            transform_c btfr{tfr.position, tfr.rotation, scale*.2f};
            Renderer2D::drawRect(btfr);
            transform_c ftfr{tfr.position + Math::getCameraForward(tfr)*.1f, tfr.rotation, scale * 0.3f};
            Renderer2D::drawRect(ftfr);
            Renderer2D::drawLine(btfr.position, ftfr.position);
            Renderer2D::drawSprite(btfr, {}
#ifdef EDITOR_BUILD
                    , (u32)id
#endif
                    );
        } else {
            f32 sz = cam.getOrtho_size() * 2;
            Renderer2D::drawRect(tfr.position, tfr.rotation.z,
                    vec2(sz * cam.getOrtho_aspect(), sz));
        }
    }

    void Scene::onEditorBeginRender(camera_c& camera, mat4 transform){
        EV_profile_function();

        Renderer2D::beginScene(camera, transform);
    }

    void Scene::onEditorRender(bool renderPhysicsShapes){
        EV_profile_function();

        auto sprgrp = _registry.group<spriteRenderer_c>(entt::get<transform_c>);
        for(auto ent : sprgrp){
            const auto& [spr, tfr] = sprgrp.get(ent);
            Renderer2D::drawSprite(tfr, spr
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
        }

        auto cirgrp = _registry.group<circleRenderer_c>(entt::get<transform_c>);
        for(auto ent : cirgrp){
            const auto& [cir, tfr] = cirgrp.get(ent);
            Renderer2D::drawCircle(tfr, cir.color, cir.thickness, cir.fade
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
        }


        auto camgrp = _registry.group<camera_c>(entt::get<transform_c>);
        for(auto ent : camgrp){
            const auto& [cam, tfr] = camgrp.get(ent);
            drawCameraGizmo(tfr, cam, (u32)ent);
        }

        if(renderPhysicsShapes){
            constexpr vec4 _phyColor = vec4(0.f, 1.f, 0.7f, 1.f);
            auto boxClds = _registry.group<boxCollider2d_c>(entt::get<transform_c>);
            for(auto ent : boxClds){
                const auto& [box, tfr] = boxClds.get(ent);
                Renderer2D::drawRect(
                        tfr.position + vec3(box.box.offset, 0.001f),
                        glm::radians(tfr.rotation.z),
                        tfr.scale * vec3(box.box.halfExtents*2.f, 1.f),
                        _phyColor
                    );
            }

            auto cirClds = _registry.group<circleCollider2d_c>(entt::get<transform_c>);
            for(auto ent : cirClds){
                const auto& [cir, tfr] = cirClds.get(ent);
                Renderer2D::drawCircle(
                        tfr.position + vec3(cir.circle.offset, 0.001f),
                        glm::max(tfr.scale.x, tfr.scale.y) * cir.circle.radius * 2.f,
                        _phyColor,
                        0.04f
                    );
            }
        }

    }

    void Scene::onEditorEndRender(){
        EV_profile_function();
        Renderer2D::endScene();
    }

    void Scene::onScenePlay(){
        EV_profile_function();
        PhysicsHandler::init(6);
    }

    void Scene::onSceneStop(){
        EV_profile_function();
        PhysicsHandler::quit();
    }

    void Scene::onUpdate(){ 
        EV_profile_function();

        PhysicsHandler::simulate(*this, Time::getDeltatime());

        _registry.view<nativeScript_c>().each([=](auto ent, nativeScript_c& nscript){
                if(!nscript._instance){
                    nscript._instance = nscript.create();
                    nscript._instance->_entity = {ent, this};
                    nscript._instance->onCreate();
                }
                nscript._instance->onUpdate();
            });
    }

    void Scene::onViewportResize(uvec2 viewportSize){
        EV_profile_function();


        _viewportSize = viewportSize;
        f32 aspect = (float)viewportSize.x / viewportSize.y;

        auto cams = _registry.view<camera_c>();
        for(auto ent : cams){
            auto& cam = cams->get(ent);
            if(!cam.fixedAspect){
                cam.setOrtho_aspect(aspect);
                cam.setPersp_aspect(aspect);
            }
        }
    }

    template<typename Comp>
    void copyComponent(entt::registry& src, entt::entity srcID, entt::registry& dest, entt::entity destID){
        if(!src.all_of<Comp>(srcID)) return;
        Comp& sc = src.get<Comp>(srcID);
        Comp& dc = dest.all_of<Comp>(destID) ?
            dest.get<Comp>(destID) : dest.emplace<Comp>(destID, sc);
        dc = sc;
    }

    ref<Scene> Scene::copy(ref<Scene>& other){
        ref<Scene> newScene = createRef<Scene>(other->_name);
        newScene->_viewportSize = other->_viewportSize;

        entt::registry& srcReg = other->_registry;
        entt::registry& desReg = newScene->_registry;

        auto idv = srcReg.view<id_c>();
        for(auto it = idv.rbegin(); it != idv.rend(); ++it){
            entt::entity e = *it;
            entt::entity id = newScene->createEntityUUID(srcReg.get<id_c>(e).id, srcReg.get<tag_c>(e).tag.c_str());
            // TODO: copy each components into the entity in new Scene
            copyComponent<transform_c>(srcReg, e, desReg, id);
            copyComponent<spriteRenderer_c>(srcReg, e, desReg, id);
            copyComponent<circleRenderer_c>(srcReg, e, desReg, id);
            copyComponent<camera_c>(srcReg, e, desReg, id);
            copyComponent<rigidbody2d_c>(srcReg, e, desReg, id);
            copyComponent<rigidbody_c>(srcReg, e, desReg, id);
            copyComponent<springJoint_c>(srcReg, e, desReg, id);
            copyComponent<springJoint2d_c>(srcReg, e, desReg, id);
            copyComponent<boxCollider2d_c>(srcReg, e, desReg, id);
            copyComponent<circleCollider2d_c>(srcReg, e, desReg, id);
        }

        return newScene;
    }
}
