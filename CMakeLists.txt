cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(everest)
message("Setting up the workspace for ${PROJECT_NAME}")

# set(STATIC_LIBS  "-static -static-libstdc++ -static-libgcc")
# set(CMAKE_EXE_LINKER_FLAGS "${STATIC_LIBS}")


# now we'll add subprojects
set(OUT_dir "${CMAKE_BINARY_DIR}/bin")

# Set build directory for everest-engine library
set(Engine "Everest")
set(ENGINE_BDIR "${OUT_dir}/${Engine}")
file(MAKE_DIRECTORY ${ENGINE_BDIR})
add_definitions(-DEDITOR_BUILD)

add_subdirectory(everest-engine ${ENGINE_BDIR})


set(everest_editor "everest-editor")
set(ENGINE_BUILD_DIR "${OUT_dir}/${everest_editor}")
file(MAKE_DIRECTORY ${ENGINE_BUILD_DIR})
add_subdirectory(everest-editor ${ENGINE_BUILD_DIR})


include(CMakePackageConfigHelpers)

set(CPACK_BUILD_TYPE ${CMAKE_BUILD_TYPE})

set(CPACK_PACKAGE_NAME "EverestEngine")
set(CPACK_PACKAGE_VENDOR "sunxsapk")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "A cross-platform high performance Game Engine in C++.")

set(CPACK_PACKAGE_CONTACT "sunx.sapk@gmail.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://sunxsapk.github.io/everest")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

if(WIN32)
    # nothing here for now
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    install(DIRECTORY everest-assets/ DESTINATION ${CMAKE_INSTALL_BINDIR}/everest-assets)

    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/everest-assets/icons/everest_installer.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/everest-assets/icons/everest_uninstaller.ico")
    set(CPACK_NSIS_WELCOME_TITLE "Everest Engine Installer")
    set(CPACK_NSIS_WELCOME_TEXT "Welcome to the Everest Engine installation wizard.")

    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

    # set(CPACK_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    unset(CPACK_PACKAGING_INSTALL_PREFIX)

elseif(APPLE) # still to test the installation on macOS
    set(CPACK_GENERATOR "productbuild")
    # set(CPACK_GENERATOR "DragNDrop" "productbuild")

    set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/everest")

    set(CPACK_PRODUCTBUILD_IDENTIFIER "com.sunxsapk.everestengine")
    set(CPACK_PRODUCTBUILD_DOMAIN "com.sunxsapk")
    set(CPACK_PRODUCTBUILD_PACKAGE "/Applications/Everest/EverestEngine.pkg")
    set(CPACK_PRODUCTBUILD_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_PRODUCTBUILD_KEYCHAIN "")
    set(CPACK_PRODUCTBUILD_SIGNING_IDENTITY "")

    set(CPACK_PACKAGING_INSTALL_PREFIX "/Applications/Everest")
    set(CPACK_PKG_INSTALL_PREFIX "/Applications/Everest")

    # install(DIRECTORY everest-assets/ DESTINATION "${CPACK_PACKAGING_INSTALL_PREFIX}/everest-assets")
    install(DIRECTORY everest-assets/ DESTINATION ${CMAKE_INSTALL_BINDIR}/everest-assets)
else()
# ======== .deb =============
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "zenity")
    install(DIRECTORY everest-assets/ DESTINATION ${CMAKE_INSTALL_BINDIR}/everest-assets)
    
# ========= .tar.gz ==========
# set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
