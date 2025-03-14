/*
 * ======== core things on library ===========
 * Author: Sunil Sapkota
 * Description : It is the main core class of the library,. It is responsible
 * for initializing, handling and destroying everything during the lifecycle of
 * the engine.
 *  ;;; Entry point to the engine is handled here ;;;
 */

#pragma once

#include <filesystem>

#ifdef DEBUG
#define _getEngineAssetsPath(path) ("everest-assets/" path)
#else
#define _getEngineAssetsPath(path) Everest::Core::getInstalledAssetsPath(path).string().c_str()
#endif

namespace Everest {


    class Core {
        public:
            static void initDependencies();
            static void init();
            static void quit();

            /*
             * returns the base-assets path of the engine
             * - Directory structure after installation looks like following:
             *      o bin
             *          o everest-assets
             *          - everest-editor (executable)
             */
            inline static std::filesystem::path getInstalledAssetsPath(std::filesystem::path path){
                return _exec_path / "everest-assets" / path;
            }

            inline static std::filesystem::path getInstalledAssetsPath(const char* path){
                return getInstalledAssetsPath(std::filesystem::path(path));
            }
            inline static std::filesystem::path getInstalledAssetsPath(std::string path){
                return getInstalledAssetsPath(std::filesystem::path(path));
            }

            inline static std::filesystem::path getExecutablePath(){
                return _exec_path;
            }
        private:
#ifdef DEBUG
            static void onGLFWerror(i32 errcode, const char *err);
#endif


            static std::filesystem::path __getExecDir();
            static std::filesystem::path _exec_path;
    };
}
