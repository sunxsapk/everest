#pragma once

#include <everest.h>


namespace Everest {
    class PropertiesPanel {
        public:
            static void onGUIrender(Entity ent);

        private:

            static void drawComponents(Entity& ent);
            static void addComponentUI(Entity& ent);

            static void drawScripts(Entity& ent);

            template<typename comp_t, typename uiDrawCallback>
            static void _componentUI(Entity& ent, const char* label, uiDrawCallback callback);

            static bool _entity(const char* label, Entity& entity);

            static void _tag(Entity& ent);
            static void _transform(Entity& ent);

            static bool _boolui(const char* label, bool& value, const char* id);
            static bool _i32ui(const char* label, i32& value, const char* id = "##0");
            static bool _f32dragui(const char* label, f32& value, f32 speed = .1f, const char* id = "##0", f32 min = 0.f, f32 max = 0.f);
            static bool _f32sliderui(const char* label, f32& value, const char* id, f32 min_ = 0.f, f32 max_ = 1.f);
            static void _vec4ui(const char* label, vec4& value, f32 resetvalue,  f32 min = 0.f, f32 max = 0.f);
            static void _vec3ui(const char* label, vec3& value, f32 resetvalue,  f32 min = 0.f, f32 max = 0.f);
            static void _vec2ui(const char* label, vec2& value, f32 resetvalue, f32 min = 0.f, f32 max = 0.f);
            static void _colorui(const char* label, vec4& value, const char* id = "");

            static bool _scriptWrapper(Scripting::scriptHandler_t& script, Entity ent);
            static void _serializeFields(Scripting::scriptHandler_t& script);
            static void scriptDropper(Entity& ent);
            // static bool _scriptHandler(Scripting::scriptHandler_t& script, Entity ent);


            constexpr static f32 colwidth = 100.f;

            friend class ScenePanel;

        private:
            static const ImGuiTreeNodeFlags _compFlags;
    };
}
