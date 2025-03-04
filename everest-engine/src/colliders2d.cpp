#include "physics/colliders2d.h"
#include "math/utils.h"
#include "scene/components.h"


namespace Everest {

    bool box2dprops_t::contains(vec2 point){
        point -= offset;

        return abs(point.x) <= halfExtents.x && abs(point.y) <= halfExtents.y;
    }

    bool circleprops_t::contains(vec2 point){
        return glm::length(point - offset) <= radius;
    }

    collider2d_c::collider2d_c(collider2d_type_t type_)
    : type(type_) {

        switch(type){
            case collider2d_type_t::Box:
                props.box = {vec2(0.f), vec2(0.5f)};
                break;

            case collider2d_type_t::Circle:
                props.circle = {vec2(0.f), 0.5f};
                break;
        }
    }

    collider2d_c::collider2d_c(const boxCollider2d_c& boxCollider, Entity entity_){
        type = collider2d_type_t::Box;
        entity = entity_;
        restitution = boxCollider.restitution;
        props.box = boxCollider.box;
    }

    collider2d_c::collider2d_c(const circleCollider2d_c& circleCollider, Entity entity_){
        type = collider2d_type_t::Circle;
        entity = entity_;
        restitution = circleCollider.restitution;
        props.circle = circleCollider.circle;
    }

    aabb2d_t getBoxAABB2D(const transform_c& parentTransform, const box2dprops_t& box){
        f32 a = glm::radians(parentTransform.rotation.z);

        vec2 e1 = parentTransform.scale;
        e1 *= box.halfExtents;
        vec2 e2(-e1.x, e1.y);
        e1 = glm::abs(Math::rotate2d(e1, a));
        e2 = glm::abs(Math::rotate2d(e2, a));

        vec2 nhe {
            glm::max(e1.x, e2.x),
            glm::max(e1.y, e2.y),
        };

        vec2 np = parentTransform.position;
        np += box.offset;
        return {
            .min = np - nhe,
            .max = np + nhe,
        };
    }

    aabb2d_t getCircleAABB2D(const transform_c& parentTransform, const circleprops_t& circle){
        vec2 np = parentTransform.position;
        np += circle.offset;
        f32 s = glm::max(parentTransform.scale.x, parentTransform.scale.y) * circle.radius;
        return {
            .min = np - s,
            .max = np + s
        };
    }

    aabb2d_t collider2d_c::getBounds(){
        switch(type){
            case collider2d_type_t::Box:
                return getBoxAABB2D(entity.get<transform_c>(), props.box);

            case collider2d_type_t::Circle:
                return getCircleAABB2D(entity.get<transform_c>(), props.circle);
        }

        return {};
    }

}
