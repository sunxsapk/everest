cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(everest VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# source files
file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
    "src/*.c"
)
file(GLOB_RECURSE EXTERNALS
    "externals/*.c"
    "externals/*.cpp"
)

# adding targets
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${EXTERNALS})

# include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    include
    externals/glad/include
)

# libraries
target_link_libraries(${PROJECT_NAME}
    glfw
)
if(APPLE)
    find_library(OPENGL_LIB OpenGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIB})
endif()

# precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
set_source_files_properties(${EXTERNALS}
    PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

# definitions
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(${PROJECT_NAME} PUBLIC LINUX)
elseif (APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC APPLE)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG;ASSERT_ON>
    $<$<CONFIG:Release>:RELEASE;ASSERT_ON>
    $<$<CONFIG:Dist>:DIST>
)

# installation setup
#install(TARGETS ${PROJECT_NAME} DESTINATION lib)
#install(DIRECTORY ${CMAKE_SOURCE_DIRECTORY}
#    DESTINATION include
#    FILES_MATCHING
#    PATTERN "*.h"
#)

