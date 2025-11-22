cmake_minimum_required(VERSION 3.25)
set(TARGET_NAME leaf)
set(CMAKE_CXX_STANDARD 20)

file(GLOB LEAF_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/core/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/events/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/events/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/imgui/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/imgui/*.cpp"
        )

aux_source_directory(platform/windows LEAF_PLATFORM_WINDOWS_SOURCE)
message(WARNING "Windows source: ${LEAF_PLATFORM_WINDOWS_SOURCE}")
aux_source_directory(platform/opengl LEAF_PLATFORM_OPENGL_SOURCE)

aux_source_directory(utils LEAF_UTILS_SOURCE)

aux_source_directory(core/renderer LEAF_RENDERER_SOURCE)

message(WARNING "Core renderer source: ${LEAF_RENDERER_SOURCE}")

add_subdirectory(third_party)
set(LEAF_ALL_SOURCE
        ${LEAF_SOURCE}
        ${LEAF_UTILS_SOURCE}
        ${LEAF_PLATFORM_WINDOWS_SOURCE}
        ${LEAF_PLATFORM_OPENGL_SOURCE}
        ${LEAF_RENDERER_SOURCE}
        )

if (BUILD_LEAF_SHARED)
    message(WARNING "Build Leaf Dynamic")
    add_library(${TARGET_NAME} SHARED
            Leaf.h
            ${LEAF_ALL_SOURCE}
            )
    target_compile_definitions(${TARGET_NAME} PRIVATE BUILD_LEAF_SHARED)
else ()
    add_library(${TARGET_NAME} STATIC
            Leaf.h
            ${LEAF_ALL_SOURCE}
            )

endif ()

target_precompile_headers(${TARGET_NAME} PRIVATE
        LeafPCH.h
        )
#add_dependencies(${TARGET_NAME} spdlog)
target_compile_definitions(${TARGET_NAME} PRIVATE BUILD_LEAF LEAF_ENABLE_ASSERT GLFW_INCLUDE_NONE)

if (USE_IMGUI_DOCKING)
    set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui_docking)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_IMGUI_DOCKING)
else ()
    set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
endif ()

target_include_directories(${TARGET_NAME} PUBLIC
        ${IMGUI_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}
        )


target_link_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_BINARY_DIR}/Leaf/third_party/spdlog
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        glfw
        opengl32
        glad
        imgui
        )

