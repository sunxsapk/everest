/*
 * ========== ui layer ===========
 * Author: Sunil Sapkota
 * Description: UI layer for the engine implements the imgui's functionality for
 * usage in engine's ui system. As of now, it is used for debugging purposes but
 * later it may be extended to render in-game ui or engine's editor.
 */

#pragma once 
#include "layer.h"

namespace Everest {
    class GUILayer : public Layer {
        public:
            GUILayer(const char* name = "GUILayer");

            void onAttach() override;
            void onDetach() override;

            void onEvent(Event& event) override;
            void onUpdate() override;

            void begin();
            void end();
        private:
            bool _blockEvents;
    };
}
