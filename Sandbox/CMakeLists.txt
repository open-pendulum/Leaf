cmake_minimum_required(VERSION 3.25)
set(TARGET_NAME sandbox)
set(CMAKE_CXX_STANDARD 20)


add_executable(${TARGET_NAME}
        SandBox.cpp
        )

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Leaf
        ${CMAKE_SOURCE_DIR}/Leaf/third_party/spdlog
        ${CMAKE_SOURCE_DIR}/Leaf/third_party/glm
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        leaf
        )