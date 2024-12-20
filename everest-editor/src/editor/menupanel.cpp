#include "menupanel.h"

namespace Everest {

    void MenuPanel::onGuiRender(){
        if(ImGui::BeginMainMenuBar()) {
            fileMenu();
            editMenu();
            ImGui::EndMainMenuBar();
        }
    }

    void MenuPanel::editMenu(){
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
    }

    inline void sceneOpen(const char* filename){
        try{
            SceneManager::loadScene(filename);
        }catch(YAML::Exception exc){
            // TODO: make this into a popup
            EVLog_Err("Error on loading scene: %s", exc.what());
        }
    }

    inline void openSceneSeq(){
        std::string scf = FileDialog::filters("*.everest");
        std::string filename = FileDialog::openFile(scf.c_str());
        if(!filename.empty()) sceneOpen(filename.c_str());
    }

    inline void openRecentSeq(){
        if(ImGui::MenuItem("scene.everest")) sceneOpen("assets/scenes/scene.everest");
        ImGui::EndMenu();
    }

    inline void saveSeq(){
        SceneManager::saveScene("assets/scenes/scene.everest");
    }

    inline void saveAsSeq(){
        std::string scf = FileDialog::filters("*.everest");
        std::string filename = FileDialog::saveFile(scf.c_str());
        if(!filename.empty()) SceneManager::saveScene(filename.c_str());
    }

    inline void newSceneSeq(){
        SceneManager::createAndActivateScene();
    }

    bool MenuPanel::onKeyShortcuts(KeyDownEvent& event){
        auto key = event.getKey();

        bool isctrl = Input::getKeyDown(K_left_control) || Input::getKeyDown(K_right_control);
        if(!isctrl) return false;

        bool isshft = Input::getKeyDown(K_left_shift) || Input::getKeyDown(K_right_shift);

        switch(key){
            case K_n:
                newSceneSeq();
                break;
            case K_o:
                openSceneSeq();
                break;
            case K_s:
                if(isshft) saveAsSeq();
                else saveSeq();
                break;

            default:
                return false;
        }
        return true;
    }

    void MenuPanel::fileMenu(){
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) newSceneSeq();
            ImGui::Separator();

            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) openSceneSeq();
            if (ImGui::BeginMenu("Open Recent Scene")) openRecentSeq();
            ImGui::Separator();

            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) saveSeq();
            if (ImGui::MenuItem("Save Scene As..",  "Ctrl+Shift+S")) saveAsSeq();
            ImGui::Separator();

            if(ImGui::MenuItem("Exit")) Application::close();
            ImGui::EndMenu();
        }
    }
}
