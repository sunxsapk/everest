#include "contentBrowserPanel.h"

namespace Everest {

    ContentBrowser* ContentBrowser::_instance = nullptr;

    void ContentBrowser::init(){
        _instance = new ContentBrowser();

        _instance->assetsDir = "assets";
        _instance->curDir = _instance->assetsDir;

        SpriteSheet icons("assets/sprites/icons.png", {128, 128});
        _instance->i_directory = icons.getSprite({0, 7}, {1, 1});
        _instance->i_file = icons.getSprite({1, 7}, {1, 1});
        _instance->i_scene = icons.getSprite({2, 7}, {1, 1});
    }

    void ContentBrowser::quit(){
        delete _instance;
    }

    void ContentBrowser::onGUIrender(){
        ImGui::Begin("Assets");

        if(_instance->curDir != _instance->assetsDir){
            if(ImGui::Button("Back")) _instance->curDir = _instance->curDir.parent_path();
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

        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(_instance->curDir)){
            std::filesystem::path path = entry.path();
            std::string filename = path.filename().string();

            ImGui::PushID(filename.c_str());
            Sprite& icon = _getIconForEntry(entry);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton("", icon.texture->getID(), {iconSize, iconSize},
                    {icon.startUV.x, icon.sizeUV.y}, {icon.sizeUV.x, icon.startUV.y});

            if (ImGui::BeginDragDropSource()) {
                std::string itemPath = path.string();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath.c_str(), itemPath.length() + 1);
				ImGui::EndDragDropSource();
			}

            // TODO: drag and drop
            ImGui::PopStyleColor();

            if( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
                if(entry.is_directory()) _instance->curDir = path;
                else if(path.extension() == ".everest"){
                    try {
                        SceneManager::loadScene(path.c_str());
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

    Sprite& ContentBrowser::_getIconForEntry(const std::filesystem::directory_entry& entry){
        if(entry.is_directory()) return _instance->i_directory;
        std::filesystem::path path = entry.path();
        if(path.extension() == ".everest") return _instance->i_scene;
        return _instance->i_file;
    }

}
