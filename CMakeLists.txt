cmake_minimum_required (VERSION 3.8)

project("Burst Enine")

file(GLOB SRC
     "*.h"
     "*.hpp"
     "*.c"
     "*.cpp"
)

add_executable (CMakeTarget ${SRC})

set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw)
target_link_libraries(CMakeTarget glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(CMakeTarget OpenGL::GL)

add_subdirectory(libs/glew/build/cmake)
target_link_libraries(CMakeTarget glew_s)
include_directories("libs/glew/include")

add_subdirectory(libs/glm)
include_directories("libs/glm")

add_subdirectory(libs/freetype)
target_link_libraries(CMakeTarget freetype)
include_directories("libs/freetype/include")

include_directories("libs/irrKlang/include")