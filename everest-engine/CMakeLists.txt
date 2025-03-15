cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(everest VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# source files
file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE EXTERNALS
    "externals/sources/*.c"
    "externals/sources/*.cpp"
)

# submodules
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory("externals/yaml-cpp")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # Ensure it's a static library

add_subdirectory("externals/glfw")

add_subdirectory("externals/lua")

# adding targets
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${EXTERNALS})

# include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    include
    externals/sources/sol
    externals/sources/glad/include
    externals/sources/imgui/include
    externals/sources/stb_image/include
    externals/sources/entt/include
    externals/sources/glm
    externals/sources/ImGuizmo
    externals/yaml-cpp/include
    externals/glfw/include
    externals/lua/src
)

# libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw yaml-cpp lua)

# precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
set_source_files_properties(${EXTERNALS} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

# definitions
if(WIN32)
    message(STATUS "Configuring project for Windows")
    # target_include_directories(${PROJECT_NAME} PUBLIC C:/GLFW/glfw/include) # if glfw link issue, then update this path
    # target_link_directories(${PROJECT_NAME} PUBLIC C:/GLFW/glfw/lib)

    target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring project for Linux")
    target_compile_definitions(${PROJECT_NAME} PUBLIC LINUX)
elseif (APPLE)
    message(STATUS "Configuring project for MacOS")
    add_compile_options(-Wno-unknown-warning-option)
    target_compile_definitions(${PROJECT_NAME} PUBLIC APPLE)
    find_library(OPENGL_LIB OpenGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIB})
    target_include_directories(${PROJECT_NAME} PUBLIC /opt/homebrew/include /usr/local/include)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG;ASSERT_ON;EV_PROFILE;__NO_3D__>
    $<$<CONFIG:Release>:RELEASE;__NO_3D__>
    $<$<CONFIG:Dist>:DIST;__NO_3D__>
)


install(TARGETS ${PROJECT_NAME}
    EXPORT EverestTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include/everest)

