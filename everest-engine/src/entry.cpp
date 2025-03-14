#include "core/entry.h"


#include "core/core.h"
#include "core/instrumentor.h"
#include "core/application.h"
#include <filesystem>

extern Everest::Application* Everest::createApplication(Everest::CommandLineArgs args);

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#endif

int main(int argc, char** argv){
    using namespace Everest;

    if(argc > 1){
        std::filesystem::path pdir(argv[1]);
        if(!std::filesystem::is_directory(pdir)){
            pdir = pdir.parent_path();
        }
        if(std::filesystem::exists(pdir)){
            std::filesystem::current_path(pdir);
            Core::setProjectDir(std::filesystem::absolute(pdir));
        }
    }

#ifdef EV_PROFILE
    if(!std::filesystem::exists("profile-results")) std::filesystem::create_directory("profile-results");
#endif

    EV_profile_begin("Everest Begin", "profile-results/result_start.json");
    Everest::Application *app = Everest::createApplication({argc, argv});
    EV_profile_end();

    EV_profile_begin("Everest Run", "profile-results/result_run.json");
    app->run();
    EV_profile_end();

    EV_profile_begin("Everest End", "profile-results/result_end.json");
    delete app;
    EV_profile_end();

    return 0;
}
