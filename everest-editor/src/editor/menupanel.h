#pragma once


#include <everest.h>


namespace Everest {

    class MenuPanel {
        public:
            static void onGUIRender();
            static bool onKeyShortcuts(KeyDownEvent& event);

        private:
            static void fileMenu();
            static void editMenu();

    };

}
