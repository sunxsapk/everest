#pragma once

#include <everest.h>


namespace Everest {
    class PropertiesPanel {
        public:
            void onGUIrender(Entity ent);

        private:

            static void drawComponents(Entity& ent);
            static void addComponentUI(Entity& ent);

            template<typename comp_t, typename uiDrawCallback>
            static void _componentUI(Entity& ent, const char* label, uiDrawCallback callback);

            static void _tag(Entity& ent);
            static void _transform(Entity& ent);

        private:
            static void _f32dragui(const char* label, f32& value, ImVec2 size, const char* id = "##0");
            static void _f32sliderui(const char* label, f32& value, const char* id);
            static void _vec3ui(const char* label, vec3& value, f32 resetvalue);
            static void _colorui(const char* label, vec4& value);

        private:
            static const ImGuiTreeNodeFlags _compFlags;
    };
}
