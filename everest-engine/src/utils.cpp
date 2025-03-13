#include "utils/utils.h"

namespace Everest {

#ifdef WIN32
#include <commdlg.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "core/application.h"

    std::string FileDialog::openFile(const char* filters){
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window(Everest::Application::getAppWindow().getWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filters;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return std::string();
    }

    std::string FileDialog::saveFile(const char* filters){
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window(Everest::Application::getAppWindow().getWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filters;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filters
        ofn.lpstrDefExt = strchr(filters, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return std::string();
    }

#else

    std::string FileDialog::openFile(const char* filters){
        char buffer[256];
        std::string command = "zenity --file-selection --file-filter=";
        command.append(filters);
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) return std::string();
        if (fgets(buffer, sizeof(buffer), pipe)) {
            pclose(pipe);
            buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline
            return std::string(buffer);
        }
        pclose(pipe);
        return std::string();
    }

    std::string FileDialog::saveFile(const char* filters){
        char buffer[256];
        std::string command = "zenity --file-selection --save --confirm-overwrite --file-filter=";
        command.append(filters);
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) return std::string();
        if (fgets(buffer, sizeof(buffer), pipe)) {
            pclose(pipe);
            buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline
            return std::string(buffer);
        }
        pclose(pipe);
        return std::string();
    }


#endif
}
