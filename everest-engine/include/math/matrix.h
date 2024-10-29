/*
 * ======== matrix ==========
 * Author: Sunil Sapkota
 * Description: matrices are one of the most important mathematical datatype in
 * game and engine development. 4x4 matrix aka _tmat4 is the basis of projection
 * calculations in renderer. This module has some types of matrices defined and
 * also the operators required.
 */

#pragma once

#include "types.h"

namespace Everest {
    template<typename T>
    struct _tmat4 {
        T _vals[4][4] = {};
    };

    template<typename T>
    _tmat4<T> operator +(_tmat4<T>,_tmat4<T>);
    template<typename T>
    _tmat4<T> operator -(_tmat4<T>,_tmat4<T>);

    using imat4 = _tmat4<i32>;
    using  mat4 = _tmat4<f32>;
}
