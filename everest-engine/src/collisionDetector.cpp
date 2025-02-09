#include "physics/collisionDetector.h"
#include "math/utils.h"
#include "renderer/renderer2d.h"


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
        vec2 contact = normal * c2.radius + c2.offset;

        BodyContact2D bc {
            .transform1 = &t1,
            .transform2 = &t2,
            .body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
            .body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
            .contactNormal = normal,
            .ra = contact - c1.offset,
            .rb = contact - c2.offset,
            .restitution = glm::min(body1.restitution, body2.restitution),
            .penetration = c1.radius + c2.radius - len,
        };
        Renderer2D::drawLine(vec3(c2.offset, 1), vec3(contact, 1), vec4(1.f, 1.f, 0.f, 1.f));
        Renderer2D::drawLine(vec3(c1.offset, 1), vec3(contact, 1), vec4(1.f, 0.f, 0.f, 1.f));

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
        f32 penetration = c2.radius - glm::length(contact);

        if(penetration < 0.f) return false;

        vec2 normal = Math::rotate2d(contact/(c2.radius-penetration), glm::radians(t1.rotation.z));
        contact = Math::rotate2d(contact + offset, glm::radians(t1.rotation.z)) + b1.offset;

        BodyContact2D bc {
            .transform1 = &t1,
            .transform2 = &t2,
            .body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
            .body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
            .contactNormal = normal,
            .ra = contact - b1.offset,
            .rb = contact - c2.offset,
            .restitution = glm::min(body1.restitution, body2.restitution),
            .penetration = penetration,
        };

        Renderer2D::drawCircle(vec3(contact, 1), 0.2f, vec4(1.f, 0.f, 0.f, 1.f));
        Renderer2D::drawLine(vec3(contact, 1), vec3(contact + normal, 1), vec4(1.f, 0.f, 0.f, 1.f));

        results.push_back(bc);
        return true;
    }

    /*
    void getBoxAxes(f32 rotation, vec2& axis1, vec2& axis2) {
        float cosTheta = cos(rotation);
        float sinTheta = sin(rotation);

        axis1 = vec2(cosTheta, sinTheta); 
        axis2 = vec2(-sinTheta, cosTheta);
    }

    void getTransformedBoxCoords(vec2 offset, vec2 size, f32 rotation, vec2* result){
        f32 sina = sin(rotation);
        f32 cosa = cos(rotation);

        result[0] = vec2(size.x * cosa - size.y * sina, size.x * sina + size.y * cosa);
        result[1] = vec2(-size.x * cosa - size.y * sina, -size.x * sina + size.y * cosa);
        result[2] = -result[0];
        result[3] = -result[1];

        for(int i=0; i<4; i++){
            result[i] += offset;
        }
    }

    vec2 getContact(Box2DProps& box1, Box2DProps& box2, f32 box1Rotation, f32 box2Rotation){
        vec2 coords[4];
        getTransformedBoxCoords(box2.offset, box2.halfExtents, box2Rotation-box1Rotation, coords);

        vec2 cp = coords[0];
        f32 md = abs(cp.x) + abs(cp.y);
        bool isFirst = true;
        for(auto& point : coords){
            if(!box1.contains(point)) continue;
            f32 _md = abs(point.x) + abs(point.y);
            if(_md < md){
                cp = point;
                md = _md;
            }
            Renderer2D::drawCircle(vec3(Math::rotate2d(cp, box1Rotation) + box1.offset, 2.f), 0.3f, vec4(1.f, 0.f, 1.f, 1.f));
        }

        getTransformedBoxCoords(box1.offset, box1.halfExtents, box1Rotation-box2Rotation, coords);
        for(auto& point : coords){
            if(!box2.contains(point)) continue;
            f32 _md = abs(point.x) + abs(point.y);
            if(_md < md){
                cp = point;
                md = _md;
                isFirst = false;
            }
            Renderer2D::drawCircle(vec3(Math::rotate2d(cp, box2Rotation) + box2.offset, 1.f), 0.3f);
        }

        vec2 contact = isFirst ?
            Math::rotate2d(cp, box1Rotation) :
            Math::rotate2d(cp, box2Rotation) ;

        return contact;
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
        getBoxAxes(glm::radians(t1.rotation.z), axes[0], axes[1]);
        getBoxAxes(glm::radians(t2.rotation.z), axes[2], axes[3]);

        auto sepAxis = [=](vec2 axis){
            f32 pa = abs(b1.halfExtents.x * glm::dot(axes[0], axis)) + abs(b1.halfExtents.y * glm::dot(axes[1], axis));
            f32 pb = abs(b2.halfExtents.x * glm::dot(axes[2], axis)) + abs(b2.halfExtents.y * glm::dot(axes[3], axis));
            return pa+pb;
        };

        f32 minOverlap = std::numeric_limits<float>::max();
        vec2 normal;
        i32 refIndex = 0;
        for(int i=0; i<4; i++){
            vec2& axis = axes[i];
            f32 overlap = sepAxis(axis);
            f32 d = glm::dot(off, axis);

            overlap -= abs(d);
            if(overlap < 0.f) return false;
            if(overlap < minOverlap) {
                minOverlap = overlap;
                normal = d < 0.f? axis: -axis;
                refIndex = i;
            }
        }

        vec2 contact = refIndex < 2?
            getContact(b1, b2, glm::radians(t1.rotation.z), glm::radians(t2.rotation.z)):
            getContact(b2, b1, glm::radians(t2.rotation.z), glm::radians(t1.rotation.z));

        BodyContact2D bc {
            .transform1 = &t1,
                .transform2 = &t2,
                .body1 = body1.entity.has<rigidbody2d_c>()?&body1.entity.get<rigidbody2d_c>():nullptr,
                .body2 = body2.entity.has<rigidbody2d_c>()?&body2.entity.get<rigidbody2d_c>():nullptr,
                .contactNormal = normal,
                .ra = contact - b1.offset,
                .rb = contact - b2.offset,
                .restitution = glm::min(body1.restitution, body2.restitution),
                .penetration = minOverlap,
        };

        Renderer2D::drawLine(vec3(b2.offset, 1), vec3(contact, 1), vec4(1.f, 1.f, 0.f, 1.f));
        Renderer2D::drawLine(vec3(b1.offset, 1), vec3(contact, 1), vec4(1.f, 0.f, 0.f, 1.f));
        //Renderer2D::drawCircle(vec3(contact, 1), 0.2f, vec4(1.f, 0.f, 0.f, 1.f));

        results.push_back(bc);
        return true;
    }
*/


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
        Box2DProps b1 = body1.props.box;
        transform_c* t1 = &body1.entity.get<transform_c>();
        b1.halfExtents *= vec2(t1->scale.x, t1->scale.y);
        b1.offset += vec2(t1->position.x, t1->position.y);

        Box2DProps b2 = body2.props.box;
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

        BodyContact2D bc {
            .transform1 = t1,
            .transform2 = t2,
            .body1 = rb1,
            .body2 = rb2,
            .contactNormal = -normal,
            .ra = ra,
            .rb = rb,
            .restitution = glm::min(body1.restitution, body2.restitution),
            .penetration = -penetration,
        };

        Renderer2D::drawCircle(vec3(contact, 1), 0.2f, vec4(1.f, 0.f, 0.f, 1.f));
        Renderer2D::drawLine(vec3(contact, 1), vec3(contact + normal, 1), vec4(1.f, 0.f, 0.f, 1.f));

        results.push_back(bc);
        return false;
    }
}
