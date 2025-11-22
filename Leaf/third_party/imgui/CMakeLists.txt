cmake_minimum_required(VERSION 3.25)
set(TARGET_NAME imgui)
set(CMAKE_CXX_STANDARD 20)

set(IMGUI_GLFW_OPENGL3_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        )

add_library(${TARGET_NAME} STATIC
        ${IMGUI_GLFW_OPENGL3_SOURCE}
        )

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )