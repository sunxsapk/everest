#include "core/entry.h"


#include "core/instrumentor.h"
#include "core/application.h"
#include "core/log.h"

extern Everest::Application* Everest::createApplication(Everest::CommandLineArgs args);

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#endif

int main(int argc, char** argv){
    using namespace Everest;

    EVLog_Msg("Path to binary : %s", argv[0]);

#ifdef WIN32
    FILE* pipe = popen("if not exist profile-res mkdir profile-res", "r");
#else
    FILE* pipe = popen("mkdir -p profile-res", "r");
#endif

    pclose(pipe);
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
