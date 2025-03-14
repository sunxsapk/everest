#include "core/uifontmanager.h"
#include "core/core.h"

namespace Everest {
    c_str_t UIFontManager::default_regular = "_def_";
    c_str_t UIFontManager::default_bold ="_def_bold_";

    std::unordered_map<c_str_t, ImFont*> UIFontManager::_fonts;

    void UIFontManager::init(){
        ImGuiIO& io = ImGui::GetIO();
        io.FontDefault = loadFont(default_regular, _getEngineAssetsPath("fonts/JetBrainsMono-Regular.ttf"), 16.f);
        loadFont(default_bold, _getEngineAssetsPath("fonts/JetBrainsMono-Bold.ttf"), 16.f);
    }

    void UIFontManager::quit(){
        _fonts.clear();
    }

    ImFont* UIFontManager::loadFont(c_str_t name, c_str_t filepath, f32 fontSize){
        ASSERT(name != nullptr, "Invalid name for font");
        ImGuiIO& io = ImGui::GetIO();
        _fonts.try_emplace(name, io.Fonts->AddFontFromFileTTF(filepath, fontSize));
        return _fonts[name];
    }
}
