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

            /*called when a layer is attached to the application*/
            virtual void onAttach();
            /*called when a layer is detached from the application*/
            virtual void onDetach();

            /*called whenever an event(key, mouse, window) is generated*/
            virtual void onEvent(Event& event);
            /*called every frame for updating the layer*/
            virtual void onUpdate();

            /*called for rendering debug-info gui for the layer*/
            virtual void onDebugRender();

            /*returns the name of the layer*/
            const char* getName() const {return _name;}

        protected:
            const char* _name;
    };
}
