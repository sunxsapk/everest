#include "core/entry.h"


#include "core/instrumentor.h"
#include "core/application.h"

extern Everest::Application* Everest::createApplication(Everest::CommandLineArgs args);

int main(int argc, char** argv){
    EV_profile_begin("Everest Begin", "profile-res/result_begin.json");
    Everest::Application *app = Everest::createApplication({argc, argv});
    EV_profile_end();

    EV_profile_begin("Everest Run", "profile-res/result_run.json");
    app->run();
    EV_profile_end();

    EV_profile_begin("Everest End", "profile-res/result_end.json");
    delete app;
    EV_profile_end();

    return 0;
}
