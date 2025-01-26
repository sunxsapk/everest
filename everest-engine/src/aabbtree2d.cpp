#include "physics/aabbtree2d.h"

namespace Everest {
    
    bool AABB2D::overlaps(const AABB2D& other) const {
        return !(
                other.min.x > max.x ||
                other.min.y > max.y ||
                other.max.x < min.x ||
                other.max.y < min.y
            );
    }

    f32 AABB2D::area() const {
        vec2 del = max - min;
        return abs(del.x * del.y);
    }

    AABB2D AABB2D::merge(const AABB2D& a, const AABB2D& b){
        AABB2D n;

        n.min.x = glm::min(a.min.x, b.min.x);
        n.max.x = glm::max(a.max.x, b.max.x);
        n.min.y = glm::min(a.min.y, b.min.y);
        n.max.y = glm::max(a.max.y, b.max.y);

        return n;
    }

}

