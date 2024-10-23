cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(everest)
message("Setting up the workspace for ${PROJECT_NAME}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# now we'll add subprojects
set(OUT_dir "${CMAKE_BINARY_DIR}/bin")

# Set build directory for everest-engine library
set(Engine "Everest")
set(ENGINE_BDIR "${OUT_dir}/${Engine}")
file(MAKE_DIRECTORY ${ENGINE_BDIR})
add_subdirectory(everest-engine ${ENGINE_BDIR})

# Set build directory for everest-editor project
set(Editor "Editor")
set(EDITOR_BDIR "${OUT_dir}/${Editor}")
file(MAKE_DIRECTORY ${EDITOR_BDIR})
add_subdirectory(everest-editor ${EDITOR_BDIR})
