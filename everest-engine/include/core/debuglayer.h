/*
 * ========== ui layer ===========
 * Author: Sunil Sapkota
 * Description: UI layer for the engine implements the imgui's functionality for
 * usage in engine's ui system. As of now, it is used for debugging purposes but
 * later it may be extended to render in-game ui or engine's editor.
 */

#pragma once 
#include "layer.h"
#include <imgui.h>

namespace Everest {
    class DebugLayer : public Layer {
        public:
            DebugLayer(const char* name = "debugger");

            void onAttach() override;
            void onDetach() override;

            void onEvent(Event& event) override;
            void onUpdate() override;

            void begin();
            void end();
            void onDebugRender() override;
        private:
            bool _blockEvents;
    };
}
