cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(everest)
message("Setting up the workspace for ${PROJECT_NAME}")


option(Build "Build" "Release")
set(CMAKE_BUILD_TYPE ${Build})

# now we'll add subprojects
set(OUT_dir "${CMAKE_BINARY_DIR}/bin")

# Set build directory for everest-engine library
set(Engine "Everest")
set(ENGINE_BDIR "${OUT_dir}/${Engine}")
file(MAKE_DIRECTORY ${ENGINE_BDIR})
add_definitions(-DEDITOR_BUILD)

if (INSTALL_ASSETS) # Should install assets to some higher directory for access from anywhere
    if(WIN32)
        set(BASE_ASSETS_PATH "C:/ProgramData/Everest")
    elseif(APPLE)
        set(BASE_ASSETS_PATH "/Library/Application Support/Everest")
    else() # Linux and other UNIX-based systems
        set(BASE_ASSETS_PATH "/usr/local/share/everest")
    endif()

    add_compile_definitions(BASE_ASSETS_PATH=\"${BASE_ASSETS_PATH}\")
endif()

add_subdirectory(everest-engine ${ENGINE_BDIR})


set(everest_editor "everest-editor")
set(ENGINE_BUILD_DIR "${OUT_dir}/${everest_editor}")
file(MAKE_DIRECTORY ${ENGINE_BUILD_DIR})
add_subdirectory(everest-editor ${ENGINE_BUILD_DIR})


include(CMakePackageConfigHelpers)

if(WIN32)
    # nothing here for now

elseif(APPLE)
    set(CPACK_GENERATOR "PKG")
    set(CPACK_PACKAGE_NAME "everest-engine")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_CONTACT "sunxsapk@gmail.com")
    set(CPACK_PACKAGE_DESCRIPTION "Everest Engine - A custom C++ game engine")
    set(CPACK_PACKAGE_VENDOR "sunxsapk")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/Applications/Everest")
    set(CPACK_PKG_INSTALL_PREFIX "/Applications/Everest")

    include(CPack)

else()
# ======== .deb =============
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_NAME "everest-engine")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_CONTACT "sunxsapk@gmail.com")
    set(CPACK_PACKAGE_DESCRIPTION "Everest Engine - A custom C++ Game Engine")
    set(CPACK_PACKAGE_VENDOR "sunxsapk")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
    
# install(DIRECTORY assets/ DESTINATION share/everest/)

# ========= .tar.gz ==========
# set(CPACK_GENERATOR "TGZ")
endif()

install(DIRECTORY assets/ DESTINATION "${BASE_ASSETS_PATH}")
include(CPack)
