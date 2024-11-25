#include "editorlayer.h"


namespace Everest {
    class Editor : public Application {
        public:
            Editor(): Application("Everest-Editor"){
                pushLayer(new EditorLayer("Everest-Editor"));
            }
    };

    Application* createApplication(CommandLineArgs args){
        return new Editor();
    }
}
