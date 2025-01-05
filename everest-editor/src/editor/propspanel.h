#pragma once

#include <everest.h>


namespace Everest {
    class PropertiesPanel {
        public:
            static void onGUIrender(Entity ent);

        private:

            static void drawComponents(Entity& ent);
            static void addComponentUI(Entity& ent);

            template<typename comp_t, typename uiDrawCallback>
            static void _componentUI(Entity& ent, const char* label, uiDrawCallback callback);

            static void _tag(Entity& ent);
            static void _transform(Entity& ent);

        private:
            static bool _f32dragui(const char* label, f32& value, f32 speed = .1f, const char* id = "##0");
            static bool _f32sliderui(const char* label, f32& value, const char* id, f32 min_ = 0.f, f32 max_ = 1.f);
            static void _vec3ui(const char* label, vec3& value, f32 resetvalue);
            static void _colorui(const char* label, vec4& value, const char* id = "");

        private:
            static const ImGuiTreeNodeFlags _compFlags;
    };
}
