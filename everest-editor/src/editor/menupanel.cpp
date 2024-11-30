#include "menupanel.h"


namespace Everest {

    void MenuPanel::onGuiRender(){
        if(ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Scene")) {}
                ImGui::Separator();

                if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
                    std::string scf = FileDialog::filters("*.everest", "*.evsc");
                    std::string filename = FileDialog::openFile(scf.c_str());
                    if(!filename.empty()) SceneSerializer::deserialize(filename.c_str());
                }
                if (ImGui::BeginMenu("Open Recent Scene")) {
                    if(ImGui::MenuItem("scene.everest")){
                        SceneSerializer::deserialize("assets/scenes/scene.everest");
                    }
                    ImGui::EndMenu();
                }
                ImGui::Separator();

                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                    SceneSerializer::serialize("assets/scenes/scene.everest");
                }
                if (ImGui::MenuItem("Save Scene As..")) {
                    std::string scf = FileDialog::filters("*.everest", "*.evsc");
                    std::string filename = FileDialog::saveFile(scf.c_str());
                    if(!filename.empty()) SceneSerializer::serialize(filename.c_str());
                }

                ImGui::Separator();
                if(ImGui::MenuItem("Exit")) Application::close();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}
