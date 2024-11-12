cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(everest)
message("Setting up the workspace for ${PROJECT_NAME}")

# option set form commandline
# Editor : will build the everest-editor
# Game : will build the game project (tower-defense)
option(Target "Target" "Editor")

option(Build "Build" "Debug")
set(CMAKE_BUILD_TYPE ${Build})

# now we'll add subprojects
set(OUT_dir "${CMAKE_BINARY_DIR}/bin")

# Set build directory for everest-engine library
set(Engine "Everest")
set(ENGINE_BDIR "${OUT_dir}/${Engine}")
file(MAKE_DIRECTORY ${ENGINE_BDIR})
add_subdirectory(everest-engine ${ENGINE_BDIR})


if (Target STREQUAL "Editor")
# everest-editor
    set(everest_editor "everest-editor")
    set(GAME_BDIR "${OUT_dir}/${everest_editor}")
    file(MAKE_DIRECTORY ${GAME_BDIR})
    add_subdirectory(everest-editor ${GAME_BDIR})
elseif (Target STREQUAL "Game")
# game project
    set(tower_defense "tower-defense")
    set(GAME_BDIR "${OUT_dir}/${tower_defense}")
    file(MAKE_DIRECTORY ${GAME_BDIR})
    add_subdirectory(tower-defense ${GAME_BDIR})
endif()
