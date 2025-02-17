#include "physics/collisionDetector.h"
#include "math/utils.h"
#include "renderer/renderer2d.h"


namespace Everest {

    bool CollisionDetector2D::checkForContacts(ref<collider2d_c>& body1, ref<collider2d_c>& body2,
            result_t& results){
        using collisionPair_t = std::pair<collider2d_type_t, collider2d_type_t>;
        using collisionDetector_t = std::function<bool(collider2d_c&, collider2d_c&, result_t&)>;
        static const std::map<collisionPair_t, collisionDetector_t> detectors{
            {
                {collider2d_type_t::Box, collider2d_type_t::Box}, box_box
            },
            {
                {collider2d_type_t::Circle, collider2d_type_t::Circle}, circle_circle
            },
            {
                {collider2d_type_t::Circle, collider2d_type_t::Box}, circle_box
            },
            {
                {collider2d_type_t::Box, collider2d_type_t::Circle}, box_circle
            },
        };

        ASSERT(body1 != nullptr && body2 != nullptr, "Null colliders on contact checker");
        if(!body1->entity.has<rigidbody2d_c>() && !body2->entity.has<rigidbody2d_c>()) return false;
        
        const collisionPair_t _pair{body1->type, body2->type};
        auto res = detectors.find(_pair);

        return res != detectors.end() && res->second != nullptr? res->second(*body1, *body2, results) : false;
    }


    bool CollisionDetector2D::circle_circle(collider2d_c& body1, collider2d_c& body2, result_t& results){
        EVLog_Msg("circle_circle");
        circleprops_t c1 = body1.props.circle;
        transform_c& t1 = body1.entity.get<transform_c>();
        c1.offset += vec2(t1.position.x, t1.position.y);
        c1.radius *= glm::max(t1.scale.x, t1.scale.y);

        circleprops_t c2 = body2.props.circle;
        transform_c& t2 = body2.entity.get<transform_c>();
        c2.offset += vec2(t2.position.x, t2.position.y);
        c2.radius *= glm::max(t2.scale.x, t2.scale.y);

        vec2 midline = Math::rotate2d(c1.offset - c2.offset, -glm::radians(t1.rotation.z));
        f32 len = glm::length(midline);

        if(len <= std::numeric_limits<f32>::epsilon() || len > (c1.radius + c2.radius)) return false;
        midline /= len;

        vec2 normal = Math::rotate2d(midline, glm::radians(t1.rotation.z));
        vec2 contact = normal * c2.radius + c2.offset;

        body_contact2d_t bodycontact {
            .transformA = &t1,
            .transformB = &t2,
            .rigidbody2dA = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
            .rigidbody2dB = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
            .contactNormal = normal,
            .relativeContactPointA = contact - c1.offset,
            .relativeContactPointB = contact - c2.offset,
            .penetration = c1.radius + c2.radius - len,
            .restitution = glm::min(body1.restitution, body2.restitution),
            // TODO: friction from physics material
        };

#if 0
        Renderer2D::drawLine(vec3(c2.offset, 1), vec3(contact, 1), vec4(1.f, 1.f, 0.f, 1.f));
        Renderer2D::drawLine(vec3(c1.offset, 1), vec3(contact, 1), vec4(1.f, 0.f, 0.f, 1.f));
#endif

        results.push_back(bodycontact);
        return true;
    }

    bool CollisionDetector2D::box_circle(collider2d_c& body1, collider2d_c& body2, result_t& results){
        box2dprops_t b1 = body1.props.box;
        transform_c& t1 = body1.entity.get<transform_c>();
        b1.offset += vec2(t1.position.x, t1.position.y);
        b1.halfExtents *= vec2(t1.scale.x, t1.scale.y);

        circleprops_t c2 = body2.props.circle;
        transform_c& t2 = body2.entity.get<transform_c>();
        c2.offset += vec2(t2.position.x, t2.position.y);
        c2.radius *= glm::max(t2.scale.x, t2.scale.y);

        vec2 offset = Math::rotate2d(c2.offset - b1.offset, -glm::radians(t1.rotation.z));
        vec2 contact = glm::clamp(offset, -b1.halfExtents, b1.halfExtents) - offset;
        f32 penetration = c2.radius - glm::length(contact);

        if(penetration <= std::numeric_limits<f32>::epsilon()) return false;

        vec2 normal = Math::rotate2d(contact/(c2.radius-penetration), glm::radians(t1.rotation.z));
        contact = Math::rotate2d(contact + offset, glm::radians(t1.rotation.z)) + b1.offset;

        body_contact2d_t bodycontact {
            .transformA = &t1,
            .transformB = &t2,
            .rigidbody2dA = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
            .rigidbody2dB = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
            .contactNormal = normal,
            .relativeContactPointA = contact - b1.offset,
            .relativeContactPointB = contact - c2.offset,
            .penetration = penetration,
            .restitution = glm::min(body1.restitution, body2.restitution),
            // TODO: friction from physics material
        };

#if 0
        Renderer2D::drawCircle(vec3(contact, 1), 0.2f, vec4(1.f, 0.f, 0.f, 1.f));
        Renderer2D::drawLine(vec3(contact, 1), vec3(contact + normal, 1), vec4(1.f, 0.f, 0.f, 1.f));
#endif

        results.push_back(bodycontact);
        return true;
    }

    struct supporting_point_t {
        vec2 point = vec2(0.f);
        vec2 normal = vec2(0.f);
        f64 minDistance = -std::numeric_limits<f32>::max();
    };

    struct temp_edge_t {
        vec2 a;
        vec2 normal;

        static temp_edge_t fromVerticesPair(vec2 a, vec2 b){
            vec2 v = glm::normalize(a-b);
            return {
                .a = a,
                .normal = vec2(-v.y, v.x)
            };
        }
    };

    bool getClosestSupportingPoint(temp_edge_t *edges, vec2 *vertices, supporting_point_t& supportingPoint){
        for(i32 i=0; i<4; i++){
            temp_edge_t& edge = edges[i];
            supporting_point_t sup{.minDistance = 0.f};
            bool allpos = true;
            for(i32 j=0; j<4; j++){
                vec2& v = vertices[j];
                f64 dist = glm::dot(edge.normal, v-edge.a);
                if(dist < sup.minDistance) {
                    allpos = false;
                    sup = {v, edge.normal, dist};
                }
            }
            if(allpos){
                supportingPoint.minDistance = 0.f;
                return false;
            }
            
            if(sup.minDistance > supportingPoint.minDistance){
                supportingPoint = sup;
            }
        }

        return true;
    };

    inline void generateBoxEdges(vec2* vertices, temp_edge_t* result){
        for(i32 i=0; i<4; i++){
            result[i] = temp_edge_t::fromVerticesPair(vertices[i], vertices[i==3?0:i+1]);
        }
    }

    void getTransformedBoxCoords(vec2 offset, vec2 size, f32 rotation, vec2* result){
        f32 sina = sin(rotation);
        f32 cosa = cos(rotation);

        result[0] = vec2( size.x * cosa - size.y * sina,  size.x * sina + size.y * cosa);
        result[1] = vec2(-size.x * cosa - size.y * sina, -size.x * sina + size.y * cosa);
        result[2] = -result[0];
        result[3] = -result[1];

        for(int i=0; i<4; i++){
            result[i] += offset;
        }
    }

    bool CollisionDetector2D::box_box(collider2d_c& body1, collider2d_c& body2, result_t& results){
        box2dprops_t b1 = body1.props.box;
        transform_c* t1 = &body1.entity.get<transform_c>();
        b1.halfExtents *= vec2(t1->scale.x, t1->scale.y);
        b1.offset += vec2(t1->position.x, t1->position.y);

        box2dprops_t b2 = body2.props.box;
        transform_c* t2 = &body2.entity.get<transform_c>();
        b2.halfExtents *= vec2(t2->scale.x, t2->scale.y);
        b2.offset += vec2(t2->position.x, t2->position.y);


        vec2 verticesA[4];
        temp_edge_t edgesA[4];
        getTransformedBoxCoords(b1.offset, b1.halfExtents, glm::radians(t1->rotation.z), verticesA);
        generateBoxEdges(verticesA, edgesA);

        vec2 verticesB[4];
        temp_edge_t edgesB[4];
        getTransformedBoxCoords(b2.offset, b2.halfExtents, glm::radians(t2->rotation.z), verticesB);
        generateBoxEdges(verticesB, edgesB);

        supporting_point_t resultA, resultB;
        bool _fa = getClosestSupportingPoint(edgesA, verticesB, resultA);
        bool _fb = getClosestSupportingPoint(edgesB, verticesA, resultB);
        if(!_fa || !_fb) return false;

        rigidbody2d_c *rb1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr;
        rigidbody2d_c *rb2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr;

        vec2 contact = resultA.point;
        vec2 normal = resultA.normal;
        vec2 ra = contact - b1.offset;
        vec2 rb = contact - b2.offset;
        f32 penetration = resultA.minDistance;

        if(resultB.minDistance > penetration){
            contact = resultB.point;
            normal = resultB.normal;
            penetration = resultB.minDistance;

            std::swap(t1, t2);
            std::swap(rb1, rb2);
            ra = contact - b2.offset;
            rb = contact - b1.offset;
        }

        body_contact2d_t bodycontact {
            .transformA = t1,
            .transformB = t2,
            .rigidbody2dA = rb1,
            .rigidbody2dB = rb2,
            .contactNormal = -normal,
            .relativeContactPointA = ra,
            .relativeContactPointB = rb,
            .penetration = -penetration,
            .restitution = glm::min(body1.restitution, body2.restitution),
            // TODO: friction from physics material
        };

#if 0
        Renderer2D::drawCircle(vec3(contact, 1), 0.2f, vec4(1.f, 0.f, 0.f, 1.f));
        Renderer2D::drawLine(vec3(contact, 1), vec3(contact + normal, 1), vec4(1.f, 0.f, 0.f, 1.f));
#endif

        results.push_back(bodycontact);
        return false;
    }
}
