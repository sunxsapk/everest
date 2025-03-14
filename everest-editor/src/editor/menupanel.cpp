#include "menupanel.h"
#include "scenepanel.h"

namespace Everest {

    void MenuPanel::onGUIRender(){
        if(ImGui::BeginMainMenuBar()) {
            fileMenu();
            editMenu();
            ImGui::EndMainMenuBar();
        }
    }

    void MenuPanel::editMenu(){
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X", false, false)) {}
            if (ImGui::MenuItem("Copy", "CTRL+C", false, false)) {}
            if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
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
        // TODO: remove or implement it on editor_state_manager type of thing later
        //if(ImGui::MenuItem("Pool Game")) sceneOpen("assets/scenes/pool_game.everest");
        //if(ImGui::MenuItem("Physics Test")) sceneOpen("assets/scenes/physicstest2d.everest");
        ImGui::EndMenu();
    }

    inline void saveAsSeq(){
        std::string scf = FileDialog::filters("*.everest");
        std::string filename = FileDialog::saveFile(scf.c_str());
        if(!filename.empty()) SceneManager::saveScene(filename.c_str());
    }

    inline void saveSeq(){
        if(SceneManager::hasSaveTarget()) SceneManager::saveScene();
        else saveAsSeq() ;
    }

    inline void newSceneSeq(){
        SceneManager::createAndActivateScene();
    }

    bool MenuPanel::onKeyShortcuts(KeyDownEvent& event){
        auto key = event.getKey();

        bool isctrl = Input::getKeyDown(K_left_control) || Input::getKeyDown(K_right_control);
        if(!isctrl) return false;

        bool isshft = Input::getKeyDown(K_left_shift) || Input::getKeyDown(K_right_shift);

        if(ScenePanel::getSceneState() != SceneState::EDIT){
            EVLog_Wrn("Cannot modify or save the scene during runtime");
            return false;
        }

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
