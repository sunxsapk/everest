#include "statspanel.h"


namespace Everest {

    void StatsPanel::onGUIrender(){
        ImGui::Begin("Stats Panel");
        gpuStats();
        rendererStats();
        frameStats();
        ImGui::End();
    }

    void StatsPanel::gpuStats(){
        if(ImGui::CollapsingHeader("GPU Stats")){

            ImGui::Text("Vendor         : %s", glGetString(GL_VENDOR));
            ImGui::Text("Renderer       : %s", glGetString(GL_RENDERER));
            ImGui::Text("Version        : %s", glGetString(GL_VERSION));

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }
    }

    void StatsPanel::rendererStats(){
        if(ImGui::CollapsingHeader("Renderer Stats")){
            RendererStats stats = Renderer2D::getStats();
            ImGui::Text("Quad Count     : %d", stats.quadCount);
            ImGui::Text("Line Count     : %d", stats.lineCount);
            ImGui::Text("VertexCount    : %d", stats.vertexCount);
            ImGui::Text("TextureCount   : %d", stats.textureCount);
            ImGui::Text("DrawCalls      : %d", stats.drawCalls);
        }
    }

    void StatsPanel::frameStats(){
        if(ImGui::CollapsingHeader("Frame Stats")){

            ImGui::Text("FPS            : %d frames/sec", Time::getFrameRate());
            ImGui::Text("Framecount     : %d frames", Time::getFrameCount());

            ImGui::Text("unscaled-time  : %.7f sec", Time::getUnscaledTime());
            ImGui::Text("unscaled-d-time: %.7f sec", Time::getUnscaledDeltatime());

            ImGui::Text("scaled-time    : %.7f sec", Time::getTime());
            ImGui::Text("scaled-d-time  : %.7f sec", Time::getDeltatime());

            static f32 _scale = 1.f;
            ImGui::SliderFloat("Time scale", &_scale, 0.f, 10.f);
            Time::setScale(_scale);

            if(ImGui::Button("Pause")) Time::setPauseState(!Time::isPaused());
            if(ImGui::Button("Manual Tick")) Time::tickManually();
        }
    }
}
