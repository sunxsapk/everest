#include "editorlayer.h"


class Editor : public Application {
    public:
        Editor(): Application("Everest-Editor"){
            pushLayer(new EditorLayer("Everest-Editor"));
        }
};

Application* Everest::createApplication(CommandLineArgs args){
    return new Editor();
}
