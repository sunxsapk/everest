#include "projectSettings.h"


namespace Everest {

    void ProjectSettingsUI::onGUIRender(project_def_t& project){
        ImGui::Begin("Scene Sequence Editor");

        std::vector<std::filesystem::path> &scenes = SceneManager::getSceneSeq();

        static char buf[1<<8];
        memset(buf, 0, sizeof(buf));
        strcpy(buf, project.name.c_str());

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Project Name");
        ImGui::SameLine();
        if(ImGui::InputText("##xx", buf, sizeof(buf))){
            project.name = std::string(buf);
        }

        ImGui::Separator();
        ImGui::Spacing();
        _scenesReorderUI(scenes);
        ImGui::Separator();
        ImGui::Spacing();
        _sceneDropper(scenes);

        ImGui::End();
    }

    void ProjectSettingsUI::_scenesReorderUI(std::vector<std::filesystem::path> &scenes){
        ImGui::PushFont(UIFontManager::getDefaultBold());


        ImGui::Text("Scenes");
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted("Drag to reorder the items in the scene sequence.");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        ImGui::Spacing();

        ImGui::PushItemFlag(ImGuiItemFlags_AllowDuplicateId, true);
        for(int n=0; n<scenes.size(); n++){

            std::filesystem::path path = scenes[n];
            ImGui::Text("%d.", n+1);
            ImGui::SameLine();
            ImGui::Selectable(path.string().c_str());

            if(ImGui::IsItemActive() && !ImGui::IsItemHovered()){
                int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                if(n_next >= 0 && n_next < scenes.size()){
                    scenes[n] = scenes[n_next];
                    scenes[n_next] = path;
                    ImGui::ResetMouseDragDelta();
                }
            }
        }
        ImGui::PopItemFlag();
        ImGui::PopFont();
    }

    void ProjectSettingsUI::_sceneDropper(std::vector<std::filesystem::path> &scenes){

        ImVec2 region = ImGui::GetContentRegionAvail();
        ImGui::Button("Drop Scenes here to submit", ImVec2(region.x, 150));

        if (ImGui::BeginDragDropTarget()) {
            const ImGuiPayload *data =
                ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
            if (data != nullptr && data->Data != nullptr) {
                const char *path_str = (const char *)data->Data;
                std::filesystem::path path(path_str);
                if (path.extension() == AssetsManager::getScene_ext()) {
                    try {
                        scenes.push_back(path);
                    } catch (std::exception exc) {
                        // TODO: make this into a popup
                        EVLog_Err("Error while submitting scene.", exc.what());
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
}
