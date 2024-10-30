#include "core/core.h"
#include "core/dbghelper.h"
#include "core/input.h"

namespace Everest {
    void Core::initDependencies(){
        glfwSetErrorCallback(onGLFWerror);
        i32 _glfwInit = glfwInit();
        ASSERT(_glfwInit);

        EVLog_Msg("Core dependencies init");
    }

    void Core::init(){
        Input::init();

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
