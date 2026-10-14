cmake_minimum_required(VERSION 3.18)
project(GenericProject CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release for single-config generators (Makefiles, Ninja)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# MSVC: link the C runtime statically (no VC++ Redistributable needed).
# Must be set before any target is created, including GLFW.
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ---- GLM (header-only math library) ----
find_package(glm QUIET)
if(NOT glm_FOUND)
    include(FetchContent)
    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1)
    FetchContent_MakeAvailable(glm)
endif()

# ---- GLFW ----
# Local dev: use the system GLFW. CI/releases: pass -DUSE_SYSTEM_GLFW=OFF
option(USE_SYSTEM_GLFW "Use GLFW installed on the system" ON)

if(USE_SYSTEM_GLFW)
    find_package(glfw3 3.3 REQUIRED)
else()
    include(FetchContent)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    if(UNIX AND NOT APPLE)
        # X11 only (runs on Wayland via XWayland); avoids extra build deps
        set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    endif()
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4)
    FetchContent_MakeAvailable(glfw)
endif()

# ---- Dear ImGui (fetched) ----
include(FetchContent)
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw ${CMAKE_DL_LIBS})

# ---- Embed shaders ----
set(SHADER_FILES vertex.glsl fragment.glsl)
set(SHADER_GEN_DIR ${CMAKE_BINARY_DIR}/generated)

set(SHADER_CONTENT "#pragma once\nnamespace shaders {\n")
foreach(f ${SHADER_FILES})
    get_filename_component(name ${f} NAME_WE)
    set(path ${CMAKE_SOURCE_DIR}/shaders/${f})
    file(READ ${path} src)
    string(APPEND SHADER_CONTENT
        "inline constexpr const char* ${name} = R\"GLSL(${src})GLSL\";\n")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${path})
endforeach()
string(APPEND SHADER_CONTENT "}\n")

file(CONFIGURE OUTPUT ${SHADER_GEN_DIR}/shaders_embedded.h
     CONTENT "${SHADER_CONTENT}" @ONLY)

# ---- Target ----
add_executable(app
    src/main.cpp
    src/window.cpp
    src/shader.cpp
    src/mesh.cpp
    src/ui.cpp
    external/glad/src/glad.c
)

target_include_directories(app PRIVATE
    include
    external/glad/include
    ${SHADER_GEN_DIR}
)

target_link_libraries(app PRIVATE glm::glm imgui ${CMAKE_DL_LIBS})

# ---- Portability: bundle compiler runtimes (releases only) ----
option(STATIC_RUNTIME "Statically link the compiler runtime" OFF)

if(STATIC_RUNTIME)
    if(MINGW)
        target_link_options(app PRIVATE -static -static-libgcc -static-libstdc++)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND UNIX AND NOT APPLE)
        target_link_options(app PRIVATE -static-libgcc -static-libstdc++)
    endif()
endif()