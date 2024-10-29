/*
 * ======== layer =========
 * Author : Sunil Sapkota
 * Description : Application is renderer as a sequence of multiple layers stacked
 * on top of each other. This allows for specifying the sequence of which one to
 * render before or after another one.
 * Example : Game world layer is rendered first, then UI layer is rendered on top
 * of it. There can be other user layers as well as debug layers.
 */

#pragma once
#include "events/event.h"

namespace Everest {
    class Layer {
        public:
            Layer(const char* name = "name");
            virtual ~Layer();

            virtual void onAttach();
            virtual void onDetach();

            virtual void onEvent(Event& event);
            virtual void onUpdate();

            virtual void onDebugRender();

            const char* getName() const {return _name;}

        protected:
            const char* _name;
    };
}
