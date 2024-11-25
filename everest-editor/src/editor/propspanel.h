#pragma once

#include <everest.h>


namespace Everest {
    class PropertiesPanel {
        public:
            void onGUIrender(Entity ent);

        private:

            void _tag(Entity& ent);
            void _transform(Entity& ent);
            void _camera(Entity& ent);
            void _spriteRenderer(Entity& ent);
            void _nativeScript(Entity& ent);
    };
}
