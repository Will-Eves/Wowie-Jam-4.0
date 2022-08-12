#pragma once

// Vendor Libs

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

// STD Libs

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>

// Engine Libs

#define STB_IMAGE_IMPLEMENTATION
#include "Engine/File/stb_image.h"
#include "Engine/File/obj_loader.h"

#include "Engine/World/Vector2.h"
#include "Engine/World/Vector3.h"
#include "Engine/World/Transform.h"

#include "Engine/ECS/ECS.h"

#include "Engine/App/App.h"
#include "Engine/App/Window.h"

#include "Engine/Input/Input.h"

#include "Engine/Rendering/Color.h"
#include "Engine/Rendering/Texture.h"
#include "Engine/Rendering/AttribObject.h"
#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/VertexObject.h"
#include "Engine/Rendering/IndexObject.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/RenderObject.h"
#include "Engine/Rendering/Enviornment.h"
#include "Engine/Rendering/Material.h"

// All components
#include "Engine/ECS/Camera.h"
#include "Engine/ECS/MeshRenderer.h"

#include "Engine/Engine/Engine.h"