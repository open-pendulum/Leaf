cmake_minimum_required(VERSION 3.25)
project(Leaf)

option(USE_IMGUI_DOCKING "Usage ImGui Docking" ON)
option(BUILD_LEAF_SHARED "Build Leaf Dynamic" OFF)

if (WIN32)
    message(WARNING "Current platform is Windows")
    add_definitions(-DLEAF_WINDOWS)
    if (MSVC)
        add_compile_options(/utf-8)
    endif ()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
        message(STATUS "Debug build")
    endif ()

elseif (APPLE)
    message(WARNING "Current platform is Apple")
    add_definitions(-DLEAF_APPLE)
elseif (UNIX)
    message(WARNING "Current platform is Unix")
    add_definitions(-DLEAF_UNIX)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(COMMON_THIRD_PARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
include_directories(${COMMON_THIRD_PARTY_PATH}/glfw/include)

add_subdirectory(third_party)
add_subdirectory(Leaf)
add_subdirectory(Sandbox)
