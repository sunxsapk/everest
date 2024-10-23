#include "core/log.h"

#include <cstdarg>
#include <cstdio>

#define __WARN_CODE "\x1b[93m"
#define __ERR_CODE "\x1b[91m"
#define __RST_CODE "\x1b[39m\n"

i32 Everest::Debug::Log(const char* format, ...){
    va_list arg;
    i32 done;
    va_start(arg, format);
    done = vfprintf(stdout, format, arg);
    va_end(arg);
    fprintf(stdout,__RST_CODE);
    return done;
}

i32 Everest::Debug::Warn(const char* format, ...){
    va_list arg;
    i32 done;
    fprintf(stdout, __WARN_CODE);
    va_start(arg, format);
    done = vfprintf(stdout, format, arg);
    va_end(arg);
    fprintf(stdout,__RST_CODE);
    return done;
}

i32 Everest::Debug::Error(const char* format, ...){
    va_list arg;
    i32 done;
    fprintf(stdout, __ERR_CODE);
    va_start(arg, format);
    done = vfprintf(stdout, format, arg);
    va_end(arg);
    fprintf(stdout,__RST_CODE);
    return done;
}
