/*
 * ======== core things on library ===========
 * Author: Sunil Sapkota
 * Description : It is the main core class of the library,. It is responsible
 * for initializing, handling and destroying everything during the lifecycle of
 * the engine.
 *  ;;; Entry point to the engine is handled here ;;;
 */

#pragma once

#ifndef ASSETS_BASEP
    #define ASSETS_BASEP "."
#endif

#define _getEngineAssetsPath(path) ASSETS_BASEP "/" path

namespace Everest {


    class Core {
        public:
            static void initDependencies();
            static void init();
            static void quit();
        private:
#ifdef DEBUG
            static void onGLFWerror(i32 errcode, const char *err);
#endif
    };
}
