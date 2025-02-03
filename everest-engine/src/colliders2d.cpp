#include "physics/colliders2d.h"
#include "scene/components.h"


namespace Everest {

    collider2d_c::collider2d_c(Collider2DType type_)
    : type(type_) {

        switch(type){
            case Collider2DType::Box:
                props.box = {vec2(0.f), vec2(0.5f)};
                break;

            case Collider2DType::Circle:
                props.circle = {vec2(0.f), 0.5f};
                break;
        }
    }

    collider2d_c::collider2d_c(const boxCollider2d_c& boxCollider, Entity entity_){
        type = Collider2DType::Box;
        entity = entity_;
        restitution = boxCollider.restitution;
        props.box = boxCollider.box;
    }

    collider2d_c::collider2d_c(const circleCollider2d_c& circleCollider, Entity entity_){
        type = Collider2DType::Circle;
        entity = entity_;
        restitution = circleCollider.restitution;
        props.circle = circleCollider.circle;
    }

    AABB2D getBoxAABB2D(transform_c& parentTransform, Box2DProps& box){
        f32 a = glm::radians(parentTransform.rotation.z);
        f32 _sin = glm::sin(a);
        f32 _cos = glm::cos(a);
        vec2 nhe = parentTransform.scale;
        nhe *= box.halfExtents;
        nhe = {
            nhe.x * _cos + nhe.y * _sin,
            nhe.x * _sin + nhe.y * _cos
        };

        vec2 np = parentTransform.position;
        np += box.offset;
        return {
            .min = np - nhe,
            .max = np + nhe,
        };
    }

    AABB2D getCircleAABB2D(transform_c& parentTransform, CircleProps& circle){
        vec2 np = parentTransform.position;
        np += circle.offset;
        f32 s = glm::max(parentTransform.scale.x, parentTransform.scale.y) * circle.radius;
        return {
            .min = np - s,
            .max = np + s
        };
    }

    AABB2D collider2d_c::getBounds(){
        switch(type){
            case Collider2DType::Box:
                return getBoxAABB2D(entity.get<transform_c>(), props.box);

            case Collider2DType::Circle:
                return getCircleAABB2D(entity.get<transform_c>(), props.circle);
        }

        return {};
    }

}
