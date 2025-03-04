cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(everest-editor VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# source files
file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
    "src/*.c"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_BINARY_DIR}
    include
)

# libraries
target_link_libraries(${PROJECT_NAME}
    everest
)
target_link_directories(${PROJECT_NAME} PUBLIC
    ${ENGINE_BDIR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG;ASSERT_ON;EV_PROFILE>
    $<$<CONFIG:Release>:RELEASE;ASSERT_ON;EV_PROFILE>
    $<$<CONFIG:Dist>:DIST>
)

