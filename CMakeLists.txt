cmake_minimum_required(VERSION 3.5)

project(Sudoku LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

add_subdirectory(external/GLFW)

add_executable(Sudoku main.cpp)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

include(GNUInstallDirs)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Sudoku")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
