/*
 * ========= debug-helpers ==============
 * Author : Sunil Sapkota
 * ===== Description =====
 * debug helpers includes the definitions for assertions and debug-breakpoints.
 */

#pragma once

#ifdef LINUX
    #define BREAKPOINT __builtin_trap()
#elif defined(APPLE)
    #define BREAKPOINT __builtin_debugtrap()
#elif defined(WIN32)
    #define BREAKPOINT __debugbreak()
#else
    #error "Unsupported OS"
#endif


#ifdef ASSERT_ON
#define ASSERT(exp, ...) if(!(exp)){\
    EVLog_Err("\n\nAssertion failed:");\
    EVLog_Err(__VA_ARGS__);\
    EVLog_Err("\t%s\
            \n\t^~~~~\
            \n=== %s, line %d ===", #exp, __FILE__, __LINE__);\
    BREAKPOINT;\
}
#else
    #define ASSERT(...)
#endif


