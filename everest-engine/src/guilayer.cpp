#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "core/guilayer.h"

namespace Everest {
    GUILayer::GUILayer(const char *name)
        : Layer(name){
    }

    void GUILayer::onAttach(){
    }

    void GUILayer::onDetach(){
    }

    void GUILayer::onEvent(Event& event){
        ImGuiIO& io = ImGui::GetIO();
        if(this->_blockEvents){
            if((event.isInCategory(Event_Mouse) && io.WantCaptureMouse) ||
            (event.isInCategory(Event_Keyboard) && io.WantCaptureKeyboard)){
                event._handled = true;
            }
        }
    }

    void GUILayer::onUpdate(){
    }

    void GUILayer::begin(){
        EV_profile_function();


        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUILayer::end(){
        EV_profile_function();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ASSERT(backup_current_context != NULL, "No window context to backing up");
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}
