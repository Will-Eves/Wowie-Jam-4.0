cmake_minimum_required (VERSION 3.8)

add_executable (CMakeTarget "main.cpp" "Burst Engine.h" "Engine/File/obj_loader.h" "Engine/File/stb_image.h" "Engine/Engine/Engine.h" "Engine/Rendering/Renderer.h" "Engine/App/Window.h" "Engine/App/App.h" "Engine/World/Vector3.h" "Engine/World/Vector2.h" "Engine/Input/Input.h" "Engine/Rendering/VertexObject.h" "Engine/Rendering/AttribObject.h" "Engine/Rendering/IndexObject.h" "Engine/Rendering/RenderObject.h" "Engine/Rendering/Shader.h" "Engine/Rendering/Mesh.h" "Engine/Rendering/Enviornment.h" "Engine/Rendering/Material.h" "Engine/Rendering/Color.h" "Engine/Rendering/Texture.h" "Engine/World/Transform.h" "Engine/ECS/ECS.h" "Engine/ECS/Camera.h" "Engine/ECS/MeshRenderer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

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