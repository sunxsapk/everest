#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "core/application.h"
#include "core/debuglayer.h"

namespace Everest {
    DebugLayer::DebugLayer(const char *name)
        : Layer(name){
    }

    void DebugLayer::onAttach(){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Window& win = Application::getWindow();
        ImGui_ImplGlfw_InitForOpenGL(win.getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410 core");
    }

    void DebugLayer::onDetach(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void DebugLayer::onEvent(Event& event){
    }

    void DebugLayer::onUpdate(){
    }

    void DebugLayer::onDebugRender(){
        ImGui::ShowDemoWindow();
        ImGui::Begin(_name);
        ImGui::Text("Hello world");
        ImGui::End();
    }

    void DebugLayer::begin(){
        ImGuiIO& io = ImGui::GetIO();
        Window& win = Application::getWindow();
        io.DisplaySize = ImVec2((f32)win.getWidth(), (f32)win.getHeight());

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
