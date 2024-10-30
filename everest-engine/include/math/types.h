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
    using namespace glm;
}
