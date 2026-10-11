cmake_minimum_required(VERSION 3.20)

project(GenericProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(test
    src/main.cpp
    external/glad/src/glad.c
)

target_include_directories(test PRIVATE
    external/glad/include
    external/GLFW
)

target_link_libraries(test PRIVATE
    glfw
    OpenGL::GL
)