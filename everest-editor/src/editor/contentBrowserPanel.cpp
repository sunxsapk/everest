#include "contentBrowserPanel.h"
#include "scenepanel.h"
#include "editorAssets.h"

namespace Everest {

    std::filesystem::path ContentBrowser::curDir = "assets";
    std::filesystem::path ContentBrowser::assetsDir = "assets";

    void ContentBrowser::onGUIrender(){
        ImGui::Begin("Assets");

        if(curDir != assetsDir){
            if(ImGui::Button("Back")) curDir = curDir.parent_path();
        } else {
            ImGui::Spacing();
        }

        constexpr f32 sliderSize = 256;
        static f32 iconSize = 128;
        f32 gap = glm::max<f32>(0.f, ImGui::GetContentRegionAvail().x - sliderSize);
        ImGui::SameLine(gap);
        ImGui::PushItemWidth(sliderSize);
        ImGui::SliderFloat("##x", &iconSize, 32, 256);
        ImGui::PopItemWidth();

        constexpr f32 padding = 16;

        f32 size = iconSize + padding;
        f32 panelWidth = ImGui::GetContentRegionAvail().x;
        i32 cols = glm::max<i32>(1, panelWidth / size);

        ImGui::Columns(cols, nullptr, false);

        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(curDir)){
            std::filesystem::path path = entry.path();
            std::string filename = path.filename().string();

            ImGui::PushID(filename.c_str());
            Sprite icon = _getIconForEntry(entry);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton("", icon.texture->getID(), {iconSize, iconSize},
                    {icon.startUV.x, icon.sizeUV.y}, {icon.sizeUV.x, icon.startUV.y});

            if (ImGui::BeginDragDropSource()) {
                std::string itemPath = path.string();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath.c_str(), itemPath.length() + 1);
				ImGui::EndDragDropSource();
			}

            ImGui::PopStyleColor();

            if( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
                if(entry.is_directory()) curDir = path;
                else if(path.extension() == AssetsManager::getScene_ext()){
                    try {
                        if(ScenePanel::getSceneState() != SceneState::EDIT) ScenePanel::onSceneStop();
                        ref<Scene> sc = AssetsManager::loadScene(path);
                        SceneManager::activateScene(sc);
                    } catch(YAML::Exception exc){
                        // TODO: make this into a popup
                        EVLog_Err("Error on loading scene: %s", exc.what());
                    }
                }
            }
            ImGui::TextWrapped("%s", filename.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::End();
    }

    Sprite ContentBrowser::_getIconForEntry(const std::filesystem::directory_entry& entry){
        if(entry.is_directory()) return EditorAssets::getIcon(IconType::DIRECTORY);
        std::filesystem::path path = entry.path();
        if(path.extension() == AssetsManager::getScene_ext()) return EditorAssets::getIcon(IconType::SCENE);
        if(AssetsManager::getAssetsType(path) == AssetsType::TEXTURE){
            return {AssetsManager::loadTexture(path)};
        }
        return EditorAssets::getIcon(IconType::FILE);
    }

}
