#include "core/entry.h"


#include "core/core.h"
#include "core/instrumentor.h"
#include "core/application.h"
#include <filesystem>

extern Everest::Application* Everest::createApplication(Everest::CommandLineArgs args);

enum class _ArgModes {
    INVALID,
    RUN_EDITOR,
    HELP,
    GENERATE_PROJECT,
    BUILD_PROJECT
};

static const char* __help_cmd = "-H";
static const char* __generate_cmd = "-G";
static const char* __build_cmd = "-B";

_ArgModes invalid_msg(const char* msg);

_ArgModes handleCommandLineArgs(i32 argc, char** argv);

_ArgModes handle2Args(i32 argc, char** argv);
_ArgModes handle3Args(i32 argc, char** argv);

void helpPage();
void generateProject();

i32 buildProject();
i32 runProject(i32 argc, char** argv);

i32 main(i32 argc, char** argv){
    switch(handleCommandLineArgs(argc, argv)){
        case _ArgModes::INVALID:
            return 1;
        case _ArgModes::GENERATE_PROJECT:
            generateProject();
            break;
        case _ArgModes::BUILD_PROJECT:
            return buildProject();
        case _ArgModes::RUN_EDITOR:
            return runProject(argc, argv);
        case _ArgModes::HELP:
            helpPage();
    }

    return 0;
}


_ArgModes invalid_msg(const char* msg){
    printf("\x1b[91m Invalid command : %s \x1b[39m", msg);
    return _ArgModes::INVALID;
}

void helpPage(){
#ifdef WIN32
    i32 _ = system("cls");
#else
    i32 _ = system("clear");
#endif
    printf(
            "\n\n"
            "============ Everest Engine - Help Page ============\n\n\n"
            "Special Commands:\n\n"
            "   -G  : Generates Project\n"
            "       - Usage    : everest-editor -G <path>\n"
            "           Generates Project files at specified <path> and loads default project template\n"
            "       - Example  : everest-editor -G Desktop/FlappyBird\n\n"
            "   -B  : Builds Standalone Project\n"
            "       - Usage    : everest-editor -B <path>\n"
            "           Builds the project located at <path> into an standalone executable\n"
            "       - Example  : everest-editor -B Desktop/FlappyBird\n\n"
            "   -H  : Displays the command-line help page for Everest Engine\n"
            "       - Usage    : everest-editor -H\n"
            "       - Example  : everest-editor -H\n\n"
            "\n"
            "Launching Project:\n\n"
            "   Method 1    : everest-editor\n"
            "               - Launches the project located at current directory in Everest Editor\n\n"
            "   Method 2    : everest-editor <path>\n"
            "               - Launches the project located at given <path> in Everest Editor\n\n"
            "   Fail Case   : This commands fails to launch the project if \"Everest.project\" file is not located at the target directory.\n"
            "\n\n\n"
            "============ Special Information =============\n\n"
            "Project Repository : https://github.com/sunxsapk/everest\n"
            "Website            : https://sunxsapk.github.io/everest\n"
            "Guide              : https://github.com/sunxsapk/everest/blob/main/docs/Guide.md\n\n"
            ">> Contact\n"
            "   Email       : sunx.sapk@gmail.com\n"
            "   Website     : https://sunxsapk.github.io\n"
            "   LinkedIn    : https://linkedin.com/in/sunxsapk\n\n"
            "====================================================\n\n\n"
          );
}

void generateProject(){
    std::filesystem::path assets_dir("assets"); 
    std::filesystem::path scenes_dir("assets/scenes"); 
    std::filesystem::path proj_file("Everest.project"); 

    printf("-- Generating project files\n");

    if(!std::filesystem::exists(assets_dir)) {
        std::filesystem::create_directory(assets_dir);
        printf("-- Created assets directory\n");
    }

    if(!std::filesystem::exists(scenes_dir)) {
        std::filesystem::create_directory(scenes_dir);
        printf("-- Created assets/scenes directory\n");
        std::filesystem::path defaultScene = _getEngineAssetsPath("scenes/default.everest");
        if(!std::filesystem::exists(scenes_dir / "scene.everest") && std::filesystem::exists(defaultScene)){
            std::filesystem::copy_file(defaultScene, (scenes_dir / "scene.everest"));
            printf("-- Loaded default scene template\n");
        }
    }

    std::filesystem::path def_proj_file = _getEngineAssetsPath("Everest.project");
    if(!std::filesystem::exists(proj_file) && std::filesystem::exists(def_proj_file)){
        std::filesystem::copy_file(def_proj_file, proj_file);
        printf("-- Created default Everest.project file\n");
    }
    printf(
            "Generated Project successfully...\n"
            "Open the generated project in editor by using command :-\n"
            ">>  everest-editor /path/to/project\n"
        );
}

i32 buildProject(){
    invalid_msg("Project building feature is not implemented yet");
    return 1;
}

_ArgModes handle2Args(i32 argc, char** argv){
    ASSERT(argc == 2, "Invalid number of arguments for executing given command");

    std::string _arg1(argv[1]);
    if(_arg1.compare(__help_cmd) == 0) return _ArgModes::HELP;
    if(_arg1.compare(__generate_cmd) == 0) return invalid_msg("Expected directory path for generating project. (None given)");
    if(_arg1.compare(__build_cmd) == 0) return invalid_msg("Expected directory path for building project. (None given)");

    std::filesystem::path pdir(argv[1]);
    if(!std::filesystem::exists(pdir)) return invalid_msg("Invalid directory for opening project");
    if(!std::filesystem::is_directory(pdir)) pdir = pdir.parent_path();
    std::filesystem::current_path(pdir);
    Everest::Core::setProjectDir(std::filesystem::absolute(pdir));
    return _ArgModes::RUN_EDITOR;
}

_ArgModes handle3Args(i32 argc, char** argv){
    ASSERT(argc == 3, "Invalid number of arguments for executing given command");

    std::string _arg1(argv[1]);
    if(_arg1.compare(__generate_cmd) == 0){
        std::filesystem::path pdir(argv[2]);
        if(!std::filesystem::exists(pdir)) {
            std::filesystem::create_directory(pdir);
        }
        if(!std::filesystem::is_directory(pdir)) {
            pdir = pdir.parent_path();
        }
        std::filesystem::current_path(pdir);
        return _ArgModes::GENERATE_PROJECT;
    }
    if(_arg1.compare(__build_cmd) == 0){
        std::filesystem::path pdir(argv[2]);
        if(!std::filesystem::exists(pdir)) return invalid_msg("Invalid path to project");
        if(!std::filesystem::is_directory(pdir)) return invalid_msg("Given Path is not a directory");
        std::filesystem::current_path(pdir);

        return _ArgModes::BUILD_PROJECT;
    }

    return _ArgModes::INVALID;
}

_ArgModes handleCommandLineArgs(i32 argc, char** argv){
    switch(argc){
        case 1:
            return _ArgModes::RUN_EDITOR;
        case 2:
            return handle2Args(argc, argv);
        case 3:
            return handle3Args(argc, argv);
        default:
            return invalid_msg("Invalid number of arguments given");
    }
}

i32 runProject(i32 argc, char** argv){
    if(!std::filesystem::exists("Everest.project")) {
        invalid_msg("\"Everest.project\" file not found in the directory.");
        return 1;
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

