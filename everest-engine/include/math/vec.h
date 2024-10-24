/*
 * ===== vectors =====
 * Author : Sunil Sapkota
 * Description : 2d, 3d and 4d vectors and their related mathematics
 */

#pragma once

namespace Everest{

    template<typename T>
    struct _tvec2 {
        T x, y;
    };

    template<typename T>
    struct _tvec3 {
        public:
            T x, y, z;
        private:
            T _pad;
    };

    template<typename T>
    struct _tvec4 {
        T x, y, z, w;
    };

    using ivec2 = _tvec2<i32>;
    using vec2  = _tvec2<f32>;

    using ivec3 = _tvec3<i32>;
    using vec3  = _tvec3<f32>;

    using ivec4 = _tvec4<i32>;
    using vec4  = _tvec4<f32>;
}
