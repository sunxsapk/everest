#include "math/utils.h"

namespace Everest {
    /*transforms point to given data, rotation is in radians*/
    vec3 transformOrtho(vec3 point, vec3 position, vec2 scale, f32 rotation){
        const f32 _sn = glm::sin(rotation);
        const f32 _cs = glm::cos(rotation);

        f32 px, py;
        px = point.x * scale.x;
        py = point.y * scale.y;
        point.x = px * _cs - py * _sn;
        point.y = px * _sn + py * _cs;

        return point + position;
    }
}
