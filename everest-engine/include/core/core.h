/*
 * ========= Core ==============
 * Author : Sunil Sapkota
 * ===== Description =====
 * Core header includes the definitions for assertions and debug-breakpoints.
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
    #define ASSERT(exp) if(!(exp)){\
    TRLog_Err(\
"Assertion Failed: %s \n\
                   ^~~~~\n\
                  === %s, line %d ===", #exp, __FILE__, __LINE__);\
        exit(1);\
}
#else
    #define ASSERT(...)
#endif

