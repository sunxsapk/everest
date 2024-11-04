/*
 * ========= Types ==============
 * Author : Sunil Sapkota
 * ===== Description =====
 * Type definitions used in engine are all included here. Mostly of the base
 * types are related to mathematical data which has to be certain bits wide.
 */

#pragma once
#include <glm/glm.hpp>

typedef signed char i8;
typedef unsigned char u8;
typedef u8 byte;

typedef signed short int i16;
typedef unsigned short int u16;

typedef signed int i32;
typedef unsigned int u32;

typedef signed long int i64;
typedef unsigned long int u64;

typedef float f32;
typedef double f64;

/* allows to use all glm types from inside engine without having to
 * scope-access them. Makes things easier.*/
namespace Everest {
    typedef glm::vec2 vec2;
    typedef glm::ivec2 ivec2;
    typedef glm::dvec2 dvec2;
    typedef glm::uvec2 uvec2;
    typedef glm::bvec2 bvec2;

    typedef glm::vec3 vec3;
    typedef glm::ivec3 ivec3;
    typedef glm::dvec3 dvec3;
    typedef glm::bvec3 bvec3;

    typedef glm::vec4 vec4;
    typedef glm::ivec4 ivec4;
    typedef glm::dvec4 dvec4;
    typedef glm::bvec4 bvec4;

    typedef glm::mat3 mat3;
    typedef glm::mat4 mat4;
}
