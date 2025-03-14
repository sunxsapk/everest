#include "physics/aabbtree2d.h"

#undef min
#undef max

namespace Everest {
    
    bool aabb2d_t::overlaps(const aabb2d_t& other) const {
        return !(
                other.min.x > max.x ||
                other.min.y > max.y ||
                other.max.x < min.x ||
                other.max.y < min.y
            );
    }

    f32 aabb2d_t::area() const {
        vec2 del = max - min;
        return abs(del.x * del.y);
    }

    aabb2d_t aabb2d_t::merge(const aabb2d_t& a, const aabb2d_t& b){
        aabb2d_t n;

        n.min.x = glm::min(a.min.x, b.min.x);
        n.max.x = glm::max(a.max.x, b.max.x);
        n.min.y = glm::min(a.min.y, b.min.y);
        n.max.y = glm::max(a.max.y, b.max.y);

        return n;
    }

}

