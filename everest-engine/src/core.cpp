#include "core/core.h"
#include "core/dbghelper.h"

namespace Everest {
    Core::Core(){
        i32 _glfwInit = glfwInit();
        ASSERT(_glfwInit);
    }

    Core::~Core(){
        glfwTerminate();
    }
}
