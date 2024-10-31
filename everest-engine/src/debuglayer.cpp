#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "core/application.h"
#include "core/debuglayer.h"

namespace Everest {
    DebugLayer::DebugLayer(const char *name, bool mViewPort)
        : Layer(name), _multiViewPorts(mViewPort){
    }

    void DebugLayer::onAttach(){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if(_multiViewPorts) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Window& win = Application::getAppWindow();
        ImGui_ImplGlfw_InitForOpenGL(win.getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void DebugLayer::onDetach(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void DebugLayer::onEvent(Event& event){
        ImGuiIO& io = ImGui::GetIO();
        if(this->_blockEvents){
            if((event.isInCategory(Event_Mouse) && io.WantCaptureMouse) ||
            (event.isInCategory(Event_Keyboard) && io.WantCaptureKeyboard)){
                event._handled = true;
            }
        }
    }

    void DebugLayer::onUpdate(){
    }

    void DebugLayer::onDebugRender(){
        ImGui::ShowDemoWindow();
        ImGui::Begin(_name);

        ImGui::Text("Vendor         : %s", glGetString(GL_VENDOR));
        ImGui::Text("Renderer       : %s", glGetString(GL_RENDERER));
        ImGui::Text("Version        : %s", glGetString(GL_VERSION));

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("FPS            : %d frames/sec", Time::getFrameRate());
        ImGui::Text("Framecount     : %d frames", Time::getFrameCount());

        ImGui::Text("unscaled-time  : %.3f sec", Time::getUnscaledTime());
        ImGui::Text("unscaled-d-time: %.3f sec", Time::getUnscaledDeltaTime());

        ImGui::Text("scaled-time    : %.3f sec", Time::getScaledTime());
        ImGui::Text("scaled-d-time  : %.3f sec", Time::getDeltaTime());

        /*
        static f32 _scale = 1.f;
        ImGui::SliderFloat("Time scale", &_scale, 0.f, 10.f);
        Time::setTimeScale(_scale);
        */

        ImGui::End();
    }

    void DebugLayer::begin(){
        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void DebugLayer::end(){
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ASSERT(backup_current_context != NULL);
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}
