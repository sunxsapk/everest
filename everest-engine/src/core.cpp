#include "core/core.h"
#include "core/dbghelper.h"
#include "core/input.h"
#include "renderer/renderer.h"
#include "core/application.h"
#include "core/time.h"
#include "core/uifontmanager.h"
#include "math/random.h"
#include "scene/scenemanager.h"
#include "utils/assetsManager.h"

namespace Everest {

    std::filesystem::path Core::_exec_path = Core::__getExecDir();

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
        UIFontManager::init();
        AssetsManager::init();
        SceneManager::init();

        Random::init();
    }

    void Core::quit(){
        EV_profile_function();

        Time::quit();
        Renderer::quit();
        UIFontManager::quit();
        AssetsManager::quit();
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


    std::filesystem::path Core::__getExecDir() {
#ifdef WIN32
        char result[MAX_PATH];
        DWORD count = GetModuleFileNameA(NULL, result, MAX_PATH);
        if(count <= 0) return std::filesystem::path("");
        std::string rr = std::string(result);
#else
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if(count == -1) return std::filesystem::path("");
        std::string rr = std::string(result, count);
#endif
        std::filesystem::path pp(rr);
        return pp.parent_path();
    }
}
