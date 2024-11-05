#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "core/debuglayer.h"
#include "core/time.h"

namespace Everest {
    DebugLayer::DebugLayer(const char *name)
        : Layer(name){
    }

    void DebugLayer::onAttach(){
    }

    void DebugLayer::onDetach(){
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
            ASSERT(backup_current_context != NULL, "No window context to backing up");
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}
