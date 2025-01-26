#include "physics/aabbtree.h"

namespace Everest {
    
    bool AABB::overlaps(const AABB& other) const {
        return !(
                other.min.x > max.x ||
                other.min.y > max.y ||
                other.min.z > max.z ||
                other.max.x < min.x ||
                other.max.y < min.y ||
                other.max.z < min.z
            );
    }

    f32 AABB::volume() const {
        vec3 del = max - min;
        return abs(del.x * del.y * del.z);
    }

    AABB AABB::merge(const AABB& a, const AABB& b){
        AABB n;

        n.min.x = glm::min(a.min.x, b.min.x);
        n.max.x = glm::max(a.max.x, b.max.x);
        n.min.y = glm::min(a.min.y, b.min.y);
        n.max.y = glm::max(a.max.y, b.max.y);
        n.min.z = glm::min(a.min.z, b.min.z);
        n.max.z = glm::max(a.max.z, b.max.z);

        return n;
    }

}
