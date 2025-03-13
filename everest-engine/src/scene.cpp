#include "scene/scene.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "renderer/renderer2d.h"
#include "math/utils.h"
#include "physics/physicsworld.h"
#include "core/time.h"


#undef min
#undef max

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

    Entity Scene::getEntityFromId(UUID id){ 
        if(entityDB.find(id) == entityDB.end()) return {};
        return Entity(entityDB[id], this);
    }

    Entity Scene::createEntityUUID(UUID id, const char * name){
        EV_profile_function();

        Entity _en {_registry.create(), this};
        _en.add<transform_c>();
        _en.add<id_c>(id);
        _en.add<tag_c>(name);
        entityDB.emplace(id, _en._id);
        return _en;
    }

    void Scene::destroyEntity(Entity& entity){
        EV_profile_function();
        
        if(!entity.isValid()) return;

        UUID id = entity.get<id_c>().id;
        onDestroyEntity(entity);
        _registry.destroy(entity._id);
        entity._id = entt::null;
        entityDB.erase(id);
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

    void Scene::onDestroyEntity(Entity& ent){
        EV_profile_function();
        if(ent == mainCamera){
            mainCamera = {};
        }
    }

    void Scene::onComponentRemoved(Entity& entity, camera_c& component){
        EV_profile_function();
        if(entity == mainCamera){
            mainCamera = {};
        }
    }

    Entity Scene::getMainCameraEntity(){
        return {mainCamera, this};
    }

    camera_c* Scene::setMainCamera(Entity& entity){
        EV_profile_function();

        if(!entity.isValid() || !entity.has<camera_c>()) return nullptr;

        mainCamera = entity;
        return &entity.get<camera_c>();
    }

    camera_c* Scene::getMainCamera() {
        Entity e = {mainCamera, this};
        return e.isValid() && e.has<camera_c>() ?
                &e.get<camera_c>() : nullptr;
    }

    void Scene::fetchTargetCamera(){
        EV_profile_function();

        auto camgrp = _registry.group<camera_c>(entt::get<transform_c>);
        for (auto ent: camgrp){
            const auto& [cam_ent, tfr] = camgrp.get(ent);
            if(cam_ent.isPrimary){
                mainCamera = ent;
                break;
            }
        }

    }

    void Scene::onRender(){ 
        EV_profile_function();

        Entity camm = {mainCamera, this};
        if(!camm.has<camera_c>()) fetchTargetCamera();
        if(!camm.has<camera_c>()) return;

        auto& camera = camm.get<camera_c>();
        if(!camera.active) return;

        Renderer2D::beginScene(camera, camm.get<transform_c>());

        auto sprgrp = _registry.group<spriteRenderer_c>(entt::get<transform_c>);
        for(auto ent : sprgrp){
            const auto& [spr, tfr] = sprgrp.get(ent);
            if(!spr.active) continue;;
            Renderer2D::drawSprite(tfr, spr
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
        }

        auto cirgrp = _registry.group<circleRenderer_c>(entt::get<transform_c>);
        for(auto ent : cirgrp){
            const auto& [cir, tfr] = cirgrp.get(ent);
            if(!cir.active) continue;;
            Renderer2D::drawCircle(tfr, cir
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
        }

        Renderer2D::endScene();
    }

    void drawCameraGizmo(transform_c& tfr, camera_c& cam_ent, u32 id){
        if(cam_ent.is3d()){
            vec3 scale(cam_ent.get_aspect(), 1.f, 1.f);
            transform_c btfr = tfr;
            tfr.scale *= 2.f;
            Renderer2D::drawRect(btfr);
            transform_c ftfr = tfr;
            ftfr.position += Math::getCameraForward(tfr)*.1f;
            ftfr.scale *= 0.3f;
            Renderer2D::drawRect(ftfr);
            Renderer2D::drawLine(btfr.position, ftfr.position);
            Renderer2D::drawSprite((mat4)btfr, {}
#ifdef EDITOR_BUILD
                    , (u32)id
#endif
                    );
        } else {
            f32 sz = cam_ent.get_lenssize() * 2;
            Renderer2D::drawRect(tfr.position, tfr.rotation.z,
                    vec2(sz * cam_ent.get_aspect(), sz));
        }
    }

    void Scene::onEditorBeginRender(camera_c& camera, mat4 transform){
        EV_profile_function();

        Renderer2D::beginScene(camera, transform);
    }

    void Scene::onEditorRender(bool renderPhysicsShapes){
        EV_profile_function();

        auto camgrp = _registry.group<camera_c>(entt::get<transform_c>);
        for(auto ent : camgrp){
            const auto& [cam_ent, tfr] = camgrp.get(ent);
            if(cam_ent.active) drawCameraGizmo(tfr, cam_ent, (u32)ent);
        }

        if(renderPhysicsShapes){
            constexpr vec4 _phyColor = vec4(0.f, 1.f, 0.7f, 1.f);
            auto boxClds = _registry.group<boxCollider2d_c>(entt::get<transform_c>);
            for(auto ent : boxClds){
                const auto& [box, tfr] = boxClds.get(ent);
                if(box.active) Renderer2D::drawRect(
                        tfr.position + vec3(box.box.offset, 0.001f),
                        glm::radians(tfr.rotation.z),
                        tfr.scale * vec3(box.box.halfExtents*2.f, 1.f),
                        _phyColor
                    );
            }

            auto cirClds = _registry.group<circleCollider2d_c>(entt::get<transform_c>);
            for(auto ent : cirClds){
                const auto& [cir, tfr] = cirClds.get(ent);
                if(cir.active) Renderer2D::drawCircle(
                        tfr.position + vec3(cir.circle.offset, 0.001f),
                        glm::max(tfr.scale.x, tfr.scale.y) * cir.circle.radius * 2.f,
                        _phyColor,
                        0.04f
                    );
            }
        }


        auto sprgrp = _registry.group<spriteRenderer_c>(entt::get<transform_c>);
        for(auto ent : sprgrp){
            const auto& [spr, tfr] = sprgrp.get(ent);
            if(spr.active) Renderer2D::drawSprite(tfr, spr
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
        }

        auto cirgrp = _registry.group<circleRenderer_c>(entt::get<transform_c>);
        for(auto ent : cirgrp){
            const auto& [cir, tfr] = cirgrp.get(ent);
            if(cir.active) Renderer2D::drawCircle(tfr, cir
#ifdef EDITOR_BUILD
                    , (u32)ent
#endif
                    );
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

        double dt = Time::getDeltatime();
        _registry.view<nativeScript_c>().each([this, dt](auto ent, nativeScript_c& nscript){
                try {
                    if(!nscript._instance){
                        nscript._instance = nscript.create();
                        nscript._instance->_entity = {ent, this};
                        nscript._instance->onCreate();
                    }
                    nscript._instance->onUpdate(dt);
                } catch (std::exception e) {
                    EVLog_Err("Error while executing native script: %s", e.what());
                }
            });

        using namespace Scripting;
        auto scripts = _registry.view<evscript_c>();
        scripts.each([this, dt](auto ent, evscript_c& nscript){
                try {
                    nscript.update(dt);
                } catch (std::exception exc) {
                    EVLog_Err("Error while executing lua script: %s", exc.what());
                }
            });

        PhysicsHandler::simulate(*this, dt);
    }

    void Scene::onViewportResize(uvec2 viewportOffset, uvec2 viewportSize){
        EV_profile_function();

        _viewportOffset = viewportOffset;
        _viewportSize = viewportSize;
        f32 aspect = (float)viewportSize.x / viewportSize.y;

        auto cams = _registry.view<camera_c>();
        for(auto ent : cams){
            auto& cam_ent = cams->get(ent);
            if(!cam_ent.fixedAspect){
                cam_ent.set_aspect(aspect);
            }
        }
    }

    vec2 Scene::worldToScreen(vec3 worldPos){
        Entity cam_ent = {mainCamera, this};
        if(!cam_ent.has<camera_c>()) fetchTargetCamera();
        if(!cam_ent.has<camera_c>()) return {};

        mat4 vpm = cam_ent.get<camera_c>().getProjection() * glm::inverse((mat4)cam_ent.get<transform_c>());
        vec4 clipc = vpm * vec4(worldPos, 1.f);
        clipc /= clipc.w;
        clipc = (clipc + 1.f) / 2.f;
        return vec2(clipc.x * _viewportSize.x + _viewportOffset.x, 
                (1.f - clipc.y) * _viewportSize.y + _viewportOffset.y);
    }

    vec3 Scene::screenToWorld(vec2 screenPos){
        Entity cam_ent = {mainCamera, this};
        if(!cam_ent.has<camera_c>()) fetchTargetCamera();
        if(!cam_ent.has<camera_c>()) return {};

        camera_c& cam = cam_ent.get<camera_c>();

        screenPos -= _viewportOffset;
        vec3 clipc(screenPos.x/_viewportSize.x*2-1.f, 1.f-screenPos.y/_viewportSize.y*2, 
                cam.is2d()? -1.f : cam.get_near());
        mat4 vpmat = cam.getProjection() * glm::inverse((mat4)cam_ent.get<transform_c>());
        vec4 pp = glm::inverse(vpmat) * vec4(clipc, 1.f);
        pp /= pp.w;
        return pp;
    }

    vec3 Scene::screenToWorldDir(vec2 screenPos){
        Entity cam_ent = {mainCamera, this};
        if(!cam_ent.has<camera_c>()) fetchTargetCamera();
        if(!cam_ent.has<camera_c>()) return {};

        screenPos -= _viewportOffset;
        vec4 ndc(screenPos.x/_viewportSize.x*2-1.f, 1.f-screenPos.y/_viewportSize.y*2, 1.f, 1.f);
        vec4 ax = glm::inverse(cam_ent.get<camera_c>().getProjection()) * ndc;
        ax /= ax.w;
        ax.w = 0.f;
        return glm::normalize((mat4)cam_ent.get<transform_c>() * ax);
    }

    template<typename Comp>
    void copyComponent(entt::registry& src, entt::entity srcID, entt::registry& dest, Entity destID){
        if(!src.all_of<Comp>(srcID)) return;
        Comp& sc = src.get<Comp>(srcID);
        Comp& dc = dest.all_of<Comp>(destID) ?
            dest.get<Comp>(destID) :
            dest.emplace<Comp>(destID);
        dc.makeCopyUsing(sc, destID);
    }

    void copyScript(entt::registry& src, entt::entity sEntity, entt::registry& dest, Entity dEntity){
        if(!src.all_of<EvScript>(sEntity)) return;
        EvScript& sc = src.get<EvScript>(sEntity);
        EvScript& dc = dest.all_of<EvScript>(dEntity) ?
            dest.get<EvScript>(dEntity) : dest.emplace<EvScript>(dEntity);
        dc.makeCopyUsing(sc, dEntity);
    }

    ref<Scene> Scene::copy(ref<Scene>& other){
        ref<Scene> newScene = createRef<Scene>(other->_name);
        newScene->_viewportSize = other->_viewportSize;
        newScene->_viewportOffset = other->_viewportOffset;

        entt::registry& srcReg = other->_registry;
        entt::registry& desReg = newScene->_registry;

        // creating and copying is done in separate steps to allow reference passing
        auto idgrp = srcReg.group<id_c>(entt::get<tag_c>);
        for(auto it = idgrp.rbegin(); it != idgrp.rend(); ++it){
            entt::entity e = *it;
            const auto& [id, tag] = idgrp.get(e);
            Entity _ent = newScene->createEntityUUID(id.id, tag.tag.c_str());
        }

        for(auto& e : idgrp){
            const auto& [id, tag] = idgrp.get(e);
            Entity _ent = newScene->getEntityFromId(id.id);
            // TODO: copy each components into the entity in new Scene
            copyComponent<transform_c>(srcReg, e, desReg, _ent);
            copyComponent<spriteRenderer_c>(srcReg, e, desReg, _ent);
            copyComponent<circleRenderer_c>(srcReg, e, desReg, _ent);
            copyComponent<camera_c>(srcReg, e, desReg, _ent);
            copyComponent<rigidbody2d_c>(srcReg, e, desReg, _ent);
            copyComponent<springJoint2d_c>(srcReg, e, desReg, _ent);
            copyComponent<boxCollider2d_c>(srcReg, e, desReg, _ent);
            copyComponent<circleCollider2d_c>(srcReg, e, desReg, _ent);

            // TODO: copy entities's reference
            copyScript(srcReg, e, desReg, _ent);

            // main camera shifting
            if(e == other->mainCamera){
                newScene->mainCamera = _ent;
            }

/*#ifndef __NO_3D__
            copyComponent<rigidbody_c>(srcReg, e, desReg, _ent);
            copyComponent<springJoint_c>(srcReg, e, desReg, _ent);
#endif*/
        }

        return newScene;
    }

    template<typename Comp>
    void duplicateComponent(Entity& src, Entity& dest){
        if(!src.has<Comp>()) return;
        Comp& sc = src.get<Comp>();
        Comp& dc = dest.tryAdd<Comp>();
        dc.makeCopyUsing(sc, dest);
    }

    Entity Scene::duplicateEntity(Entity& entity){
        Entity ent = createEntity(entity.get<tag_c>().tag.c_str());

        duplicateComponent<transform_c>(entity, ent);
        duplicateComponent<spriteRenderer_c>(entity, ent);
        duplicateComponent<circleRenderer_c>(entity, ent);
        duplicateComponent<camera_c>(entity, ent);
        duplicateComponent<rigidbody2d_c>(entity, ent);
        duplicateComponent<springJoint2d_c>(entity, ent);
        duplicateComponent<boxCollider2d_c>(entity, ent);
        duplicateComponent<circleCollider2d_c>(entity, ent);
        duplicateComponent<EvScript>(entity, ent);

/*#ifndef __NO_3D__
        duplicateComponent<rigidbody_c>(entity, ent);
        duplicateComponent<springJoint_c>(entity, ent);
#endif*/
        return ent;
    }
}
