#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "core/guilayer.h"

namespace Everest {
    GUILayer::GUILayer(const char *name)
        : Layer(name){
        setDarkThemeColors();
    }

    void GUILayer::onAttach(){
    }

    void GUILayer::onDetach(){
    }

    void GUILayer::onEvent(Event& event){
        ImGuiIO& io = ImGui::GetIO();
        event._handled |= event.isInCategory(Event_Mouse) & io.WantCaptureMouse;
        event._handled |= event.isInCategory(Event_Keyboard) & io.WantCaptureKeyboard;
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

    void GUILayer::setDarkThemeColors() {
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg]           = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header]             = ImVec4{ 0.22f, 0.20f, 0.15f, 1.0f };
		colors[ImGuiCol_HeaderHovered]      = ImVec4{ 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_HeaderActive]       = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button]             = ImVec4{ 0.41f, 0.21f, 0.11f, 1.0f };
		colors[ImGuiCol_ButtonHovered]      = ImVec4{ 0.64f, 0.32f, 0.08f, 1.0f };
		colors[ImGuiCol_ButtonActive]       = ImVec4{ 0.31f, 0.28f, 0.24f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg]            = ImVec4{ 0.14f, 0.40f, 0.34f, 1.0f };
		colors[ImGuiCol_FrameBgHovered]     = ImVec4{ 0.10f, 0.64f, 0.48f, 1.0f };
		colors[ImGuiCol_FrameBgActive]      = ImVec4{ 0.22f, 0.34f, 0.21f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab]                = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered]         = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive]          = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused]       = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg]            = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive]      = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed]   = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
}
