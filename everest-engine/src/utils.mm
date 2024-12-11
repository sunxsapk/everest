#include "utils/utils.h"



namespace Everest {

#ifdef APPLE

#include <CoreFoundation/CoreFoundation.h>
#include <AppKit/AppKit.h>

    std::string FileDialog::openFile(const char* filters){
        @autoreleasepool {
            NSOpenPanel* panel = [NSOpenPanel openPanel];
            [panel setAllowsMultipleSelection:NO];
            [panel setCanChooseFiles:YES];
            [panel setCanChooseDirectories:NO];

            if ([panel runModal] == NSModalResponseOK) {
                NSURL* fileUrl = [[panel URLs] firstObject];
                return std::string([[fileUrl path] UTF8String]);
            }
        }
        return std::string();
    }

    std::string FileDialog::saveFile(const char* filters){
        @autoreleasepool {
            NSSavePanel* panel = [NSSavePanel savePanel];
            if ([panel runModal] == NSModalResponseOK) {
                NSURL* fileUrl = [panel URL];
                return std::string([[fileUrl path] UTF8String]);
            }
        }
        return std::string();
    }
#endif
}
