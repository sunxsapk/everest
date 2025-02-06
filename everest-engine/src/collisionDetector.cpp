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
        EVLog_Msg("circle_circle");
        CircleProps c1 = body1.props.circle;
        transform_c& t1 = body1.entity.get<transform_c>();
        c1.offset += vec2(t1.position.x, t1.position.y);
        c1.radius *= glm::max(t1.scale.x, t1.scale.y);

        CircleProps c2 = body2.props.circle;
        transform_c& t2 = body2.entity.get<transform_c>();
        c2.offset += vec2(t2.position.x, t2.position.y);
        c2.radius *= glm::max(t2.scale.x, t2.scale.y);

        vec2 midline = Math::rotate2d(c1.offset - c2.offset, -glm::radians(t1.rotation.z));
        f32 len = glm::length(midline);

        if(len <= 0 || len > (c1.radius + c2.radius)) return false;
        midline /= len;

        vec2 normal = Math::rotate2d(midline, glm::radians(t1.rotation.z));

        BodyContact2D bc {
            .transform1 = &t1,
            .transform2 = &t2,
            .body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
            .body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
            .contactNormal = normal,
            .restitution = glm::min(body1.restitution, body2.restitution),
            .penetration = c1.radius + c2.radius - len,
        };
        bc.rb = normal * c2.radius;
        bc.ra = c2.offset + bc.rb - c1.offset; // not proud of this line
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
        c2.offset += vec2(t2.position.x, t2.position.y);
        c2.radius *= glm::max(t2.scale.x, t2.scale.y);

        vec2 offset = Math::rotate2d(c2.offset - b1.offset, -glm::radians(t1.rotation.z));
        vec2 contact = glm::clamp(offset, -b1.halfExtents, b1.halfExtents) - offset;
        f32 plen = c2.radius - glm::length(contact);

        if(plen < 0.f) return false;

        vec2 normal = Math::rotate2d(contact/(c2.radius-plen), glm::radians(t1.rotation.z));

        BodyContact2D bc {
            .transform1 = &t1,
            .transform2 = &t2,
            .body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
            .body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
            .contactNormal = normal,
            .restitution = glm::min(body1.restitution, body2.restitution),
            .penetration = plen,
        };
        bc.rb = normal * c2.radius;
        bc.ra = c2.offset + bc.rb - b1.offset; // not proud of this line: TODO: after penetration resolved

        results.push_back(bc);
        return true;
    }


    bool CollisionDetector2D::box_box(collider2d_c& body1, collider2d_c& body2, result_t& results){
        Box2DProps b1 = body1.props.box;
        transform_c& t1 = body1.entity.get<transform_c>();
        b1.halfExtents *= vec2(t1.scale.x, t1.scale.y);
        b1.offset += vec2(t1.position.x, t1.position.y);

        Box2DProps b2 = body2.props.box;
        transform_c& t2 = body2.entity.get<transform_c>();
        b2.halfExtents *= vec2(t2.scale.x, t2.scale.y);
        b2.offset += vec2(t2.position.x, t2.position.y);

        vec2 off = b2.offset - b1.offset;

        vec2 axes[4];
        axes[0] = Math::rotate2d(vec2(1,0), glm::radians(t1.rotation.z));
        axes[1] = vec2(-axes[0].y, axes[0].x);
        axes[2] = Math::rotate2d(vec2(1,0), glm::radians(t2.rotation.z));
        axes[3] = vec2(-axes[2].y, axes[2].x);

        auto sepAxis = [=](vec2 axis){
            f32 pa = abs(b1.halfExtents.x * glm::dot(axes[0], axis)) + abs(b1.halfExtents.y * glm::dot(axes[1], axis));
            f32 pb = abs(b2.halfExtents.x * glm::dot(axes[2], axis)) + abs(b2.halfExtents.y * glm::dot(axes[3], axis));
            return pa+pb;
        };

        f32 minOverlap = std::numeric_limits<float>::max();
        vec2 normal = axes[0];
        for(auto& axis : axes){
            f32 overlap = sepAxis(axis);
            f32 d = glm::dot(off, axis);

            overlap -= abs(d);
            if(overlap <= 0.f) return false;
            if(overlap < minOverlap) {
                minOverlap = overlap;
                normal = d<0? axis: -axis;
            }
        }

        BodyContact2D bc {
            .transform1 = &t1,
            .transform2 = &t2,
            .body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
            .body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
            .contactNormal = normal,
            .restitution = glm::min(body1.restitution, body2.restitution),
            .penetration = minOverlap,
        };
        // TODO:
        bc.rb = normal * b2.halfExtents; // not proud of this line
        bc.ra = b2.offset + bc.rb - b1.offset; // not proud of this line: TODO: after penetration resolved

        results.push_back(bc);
        return true;
    }
}
