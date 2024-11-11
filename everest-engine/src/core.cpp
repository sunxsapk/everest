#include "core/core.h"
#include "core/dbghelper.h"
#include "core/input.h"
#include "renderer/renderer.h"
#include "core/application.h"
#include "core/time.h"
#include "math/random.h"

namespace Everest {
    void Core::initDependencies(){
        EV_profile_function();


        EVLog_Msg("GLFW init");
#ifdef DEBUG
        glfwSetErrorCallback(onGLFWerror);
#endif
        i32 _glfwInit = glfwInit();
        ASSERT(_glfwInit, "Failed to initialize GLFW");

        EVLog_Msg("ImGUI init");
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
    }

    void Core::init(){
        EV_profile_function();


        EVLog_Msg("Core init");

        Window& win = Application::getAppWindow();
        ImGui_ImplGlfw_InitForOpenGL(win.getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        Input::init();
        Renderer::init();
        Time::init();
        Random::init() ;
    }

    void Core::quit(){
        EV_profile_function();

        Time::quit();
        Renderer::quit();
        EVLog_Msg("Core quit");

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        EVLog_Msg("ImGui quit");

        glfwTerminate();
        EVLog_Msg("GLFW quit");
    }

#ifdef DEBUG
    void Core::onGLFWerror(i32 errcode, const char *err){
        EVLog_Err("Opengl Error (%d): %s", errcode, err);
    }
#endif
}
