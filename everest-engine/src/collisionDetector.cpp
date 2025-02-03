#include "physics/collisionDetector.h"
#include "math/utils.h"


namespace Everest {

    bool CollisionDetector2D::checkForContacts(ref<collider2d_c>& body1, ref<collider2d_c>& body2,
            result_t& results){
        using collisionPair_t = std::pair<Collider2DType, Collider2DType>;
        using collisionDetector_t = std::function<bool(collider2d_c&, collider2d_c&, result_t&)>;
        static const std::map<collisionPair_t, collisionDetector_t> detectors{
            {
                {Collider2DType::Box, Collider2DType::Box}, box_box
            },
            {
                {Collider2DType::Circle, Collider2DType::Circle}, circle_circle
            },
            {
                {Collider2DType::Circle, Collider2DType::Box}, circle_box
            },
            {
                {Collider2DType::Box, Collider2DType::Circle}, box_circle
            },
        };

        ASSERT(body1 != nullptr && body2 != nullptr, "Null colliders on contact checker");
        
        const collisionPair_t _pair{body1->type, body2->type};
        auto res = detectors.find(_pair);

        return res != detectors.end() && res->second != nullptr? res->second(*body1, *body2, results) : false;
    }


    bool CollisionDetector2D::circle_circle(collider2d_c& body1, collider2d_c& body2, result_t& results){
        CircleProps& c1 = body1.props.circle;
        transform_c& p1 = body1.entity.get<transform_c>();
        f32 r1 = glm::max(p1.scale.x, p1.scale.y) * c1.radius;

        CircleProps& c2 = body2.props.circle;
        transform_c& p2 = body2.entity.get<transform_c>();
        f32 r2 = glm::max(p2.scale.x, p2.scale.y) * c2.radius;

        vec2 midline = vec2(p1.position.x, p1.position.y) + c1.offset - vec2(p2.position.x, p2.position.y) - c2.offset;
        f32 len = midline.length();

        if(len <= 0 || len > (r1+r2)) return false;
        midline /= len;

        BodyContact2D bc {
            .transform1 = &p1,
            .transform2 = &p2,
            .contactNormal = midline,
            .restitution = (body1.restitution + body2.restitution)/2,
            .penetration = r1 + r2 - len,
        };
        bc.body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr;
        bc.body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr;
        results.push_back(bc);
        return true;
    }

    bool CollisionDetector2D::box_circle(collider2d_c& body1, collider2d_c& body2, result_t& results){
        Box2DProps b1 = body1.props.box;
        transform_c& t1 = body1.entity.get<transform_c>();
        b1.offset += vec2(t1.position.x, t1.position.y);
        b1.halfExtents *= vec2(t1.scale.x, t1.scale.y);

        CircleProps c2 = body2.props.circle;
        transform_c& t2 = body2.entity.get<transform_c>();
        c2.radius *= glm::max(t2.scale.x, t2.scale.y);
        c2.offset += vec2(t2.position.x, t2.position.y);

        vec2 midline = Math::rotate2d(b1.offset - c2.offset, glm::radians(-t1.rotation.z));

        // TODO: penetration is not calculated efficiently
        midline -= glm::normalize(midline) * c2.radius;
        vec2 aml = glm::abs(midline);
        if(aml.x >= b1.halfExtents.x) return false;
        if(aml.y >= b1.halfExtents.y) return false;

        bool xfoc = aml.y > aml.x;
        if(xfoc) {
            midline.x = midline.x > 0? 1.f : -1.f;
            midline.y = 0.f;
        } else{
            midline.x = 0.f;
            midline.y = midline.y > 0? 1.f : -1.f;
        }

        BodyContact2D bc {
            .transform1 = &t1,
            .transform2 = &t2,
            .contactNormal = midline,
            .restitution = (body1.restitution + body2.restitution)/2,
            .penetration = xfoc ? (b1.halfExtents.x-aml.x) : (b1.halfExtents.y-aml.y)
        };
        bc.body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr;
        bc.body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr;
        results.push_back(bc);
        return true;
    }

    bool CollisionDetector2D::box_box(collider2d_c& body1, collider2d_c& body2, result_t& results){
        /*
        Box2DProps b1 = body1.props.box;
        transform_c& t1 = body1.entity.get<transform_c>();
        b1.halfExtents *= vec2(t1.scale.x, t1.scale.y);
        b1.offset += vec2(t1.position.x, t1.position.y);

        Box2DProps b2 = body2.props.box;
        transform_c& t2 = body2.entity.get<transform_c>();
        b2.halfExtents *= vec2(t2.scale.x, t2.scale.y);
        b2.offset += vec2(t2.position.x, t2.position.y);

        vec2 _tp = Math::rotate2d(vec2(1,0), glm::radians(t2.rotation.z - t1.rotation.z));
        vec2 axes[4] {
            {1, 0},
            {0, 1},
            _tp,
            {-_tp.y, _tp.x}
        };

        auto checkAxis = [&](int x){
            axes[1].x += 1;
        };
        */

        return false;
    }
}
