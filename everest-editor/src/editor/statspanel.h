#pragma once

#include <everest.h>

namespace Everest {
    class StatsPanel {
        public:
            static void onGUIrender();
        private:
            static void gpuStats();
            static void rendererStats();
            static void frameStats();
    };
}
