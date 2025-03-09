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
        set(ASSETS_BASEP "C:/ProgramData/Everest")
    elseif(APPLE)
        set(ASSETS_BASEP "/Library/Application Support/Everest")
    else() # Linux and other UNIX-based systems
        set(ASSETS_BASEP "/usr/local/share/everest")
    endif()

    add_compile_definitions(ASSETS_BASEP=\"${ASSETS_BASEP}\")
endif()

add_subdirectory(everest-engine ${ENGINE_BDIR})


set(everest_editor "everest-editor")
set(EDITOR_BDIR "${OUT_dir}/${everest_editor}")
file(MAKE_DIRECTORY ${EDITOR_BDIR})
add_subdirectory(everest-editor ${EDITOR_BDIR})

# install(DIRECTORY assets/ DESTINATION share/everest/)
install(DIRECTORY assets/ DESTINATION "${ASSETS_BASEP}")

include(CMakePackageConfigHelpers)

# install(EXPORT EverestTargets
#     FILE EverestConfig.cmake
#     NAMESPACE Everest::
#     DESTINATION lib/cmake/everest
# )


# ======== .deb =============
# set(CPACK_GENERATOR "DEB")
# set(CPACK_PACKAGE_NAME "everest-engine")
# set(CPACK_PACKAGE_VERSION "1.0.0")
# set(CPACK_PACKAGE_CONTACT "sunxsapk@gmail.com")
# set(CPACK_PACKAGE_DESCRIPTION "Everest Engine - A custom C++ Game Engine")
# set(CPACK_PACKAGE_VENDOR "sunxsapk")
# set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")

set(CPACK_GENERATOR "TGZ")

# Required for CPack to work
include(CPack)

