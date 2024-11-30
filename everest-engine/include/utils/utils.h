/*
 * =========== Utilities ==============
 * Author: Sunil Sapkota
 * Description: Contains some utilities functions and/or components for engine
 */

#pragma once

namespace Everest {

    class FileDialog {
        public:
            template<typename... args_t>
            static std::string filters(args_t... args){
                std::ostringstream res;
#ifdef LINUX
                res << "\"";
                ((res << "\"" << args << "\" "), ...);
                res << "\"";

#elif defined(WIN32)
                ((res << args << "\0"), ...);
#elif defined(APPLE)
                ((res << args << " "), ...);
#endif

                return res.str();
            }

            static std::string openFile(const char* filter = "");
            static std::string saveFile(const char* filter = "");
    };
}
