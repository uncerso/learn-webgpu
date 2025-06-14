cmake_minimum_required(VERSION 3.0...3.25)
project(
    LearnWebGPU
    VERSION 0.1.0
    LANGUAGES CXX C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/main.cpp src/core/*.cpp src/tasks/*.cpp src/runtime/**/*.cpp)
add_executable(run ${SOURCES})

set_target_properties(run PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
)

target_compile_options(run PRIVATE -Wno-deprecated-volatile -Wno-vla-extension -Werror -pedantic-errors -Wall -Wextra -Wmost -Wmove -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wextra-semi -Wfloat-equal -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo)
set(CMAKE_CXX_FLAGS_DEBUG -g)
set(CMAKE_CXX_FLAGS_RELEASE -O3)

# target_compile_options(run PRIVATE -fsanitize=address)
# target_link_options(run PRIVATE -fsanitize=address)

# target_compile_options(run PRIVATE -fsanitize=undefined)
# target_link_options(run PRIVATE -fsanitize=undefined)

add_subdirectory(webgpu)
add_subdirectory(glfw)
add_subdirectory(glfw3webgpu)
add_subdirectory(glm)

target_link_libraries(run PRIVATE glfw3webgpu glm)
target_include_directories(run PRIVATE src)
