#include "physics/colliders2d.h"
#include "math/utils.h"
#include "scene/components.h"


namespace Everest {

    bool Box2DProps::contains(vec2 point){
        point -= offset;

        return abs(point.x) <= halfExtents.x && abs(point.y) <= halfExtents.y;
    }

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

    AABB2D getBoxAABB2D(const transform_c& parentTransform, const Box2DProps& box){
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

    AABB2D getCircleAABB2D(const transform_c& parentTransform, const CircleProps& circle){
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
