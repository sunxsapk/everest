#include "core/core.h"
#include "core/dbghelper.h"

namespace Everest {
    void Core::init(){
        glfwSetErrorCallback(onGLFWerror);
        i32 _glfwInit = glfwInit();
        ASSERT(_glfwInit);
        EVLog_Msg("Core init");
    }

    void Core::quit(){
        glfwTerminate();
        EVLog_Msg("Core quit");
    }

    void Core::onGLFWerror(i32 errcode, const char *err){
        EVLog_Err("Opengl Error (%d): %s", errcode, err);
    }
}
