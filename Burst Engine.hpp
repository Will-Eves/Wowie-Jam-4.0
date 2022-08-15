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

#include <ft2build.h>
#include FT_FREETYPE_H

#pragma comment(lib, "lib/irrKlang.lib")
#include <irrKlang.h>

// STD Libs

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <ctime>
#include <omp.h>

// Engine Libs

// Not ours
#define STB_IMAGE_IMPLEMENTATION
#include "Engine/File/stb_image.hpp"
#include "Engine/File/obj_loader.hpp"

// Ours
#include "Engine/Time/Time.hpp"

#include "Engine/World/Vector2.hpp"
#include "Engine/World/Vector3.hpp"
#include "Engine/World/Transform.hpp"

#include "Engine/ECS/ECS.hpp"

#include "Engine/App/App.hpp"
#include "Engine/App/Window.hpp"

#include "Engine/Input/Input.hpp"

#include "Engine/Rendering/Color.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/AttribObject.hpp"
#include "Engine/Rendering/Shader.hpp"
#include "Engine/Rendering/VertexObject.hpp"
#include "Engine/Rendering/IndexObject.hpp"
#include "Engine/Rendering/Renderer.hpp"
#include "Engine/Rendering/Mesh.hpp"
#include "Engine/Rendering/RenderObject.hpp"
#include "Engine/Rendering/Enviornment.hpp"
#include "Engine/Rendering/Material.hpp"

#include "Engine/Audio/AudioManager.hpp"

#include "Engine/Physics/Physics.hpp"

#include "Engine/UI/AnchorPoint.hpp"
#include "Engine/UI/Font.h"
#include "Engine/UI/UIManager.hpp"
#include "Engine/UI/RectTransform.hpp"

// All components
#include "Engine/ECS/Camera.hpp"
#include "Engine/ECS/MeshRenderer.hpp"
#include "Engine/ECS/Light.hpp"
// UI Components
#include "Engine/UI/PanelRenderer.hpp"
#include "Engine/UI/TextRenderer.hpp"
#include "Engine/UI/Button.hpp"

#include "Engine/Engine/Engine.hpp"