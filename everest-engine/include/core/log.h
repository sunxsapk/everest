/*
 * ========= Log ==============
 * Author : Sunil Sapkota
 * ===== Description =====
 * Implementation of logging system:
 *  - Debug class with static methods for logging messages, warnings and errors
 *  - Definitions of engine-debug symbols separately in order to eliminate debug
 *      messages in Release build.
 */

#pragma once

#include "math/types.h"

#ifdef DEBUG
    #define TRLog_Msg(...) Debug::Log(__VA_ARGS__)
    #define TRLog_Wrn(...) Debug::Warn(__VA_ARGS__)
    #define TRLog_Err(...) Debug::Error(__VA_ARGS__)
#else
    #define TRLog_Msg(...)
    #define TRLog_Wrn(...)
    #define TRLog_Err(...)
#endif

namespace Everest {
    class Debug {
        public:
            static i32 Log(const char *str_fmt, ...);
            static i32 Warn(const char *str_fmt, ...);
            static i32 Error(const char *str_fmt, ...);
    };
}
