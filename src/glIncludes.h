#pragma once
// PFD must be first import
#include <portable-file-dialogs.h>

// ImGui needs to be imported before GLAD
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// GLAD must be imported before GLFW
#include <glad/glad.h>

// GLFW must be imported before GLM
#include <GLFW/glfw3.h>

// GLM should be imported after GLFW
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <stb_image.h>

// ASSIMP should be imported after GLM
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
constexpr float PI = 3.141592f;
constexpr float HALF_PI = 1.57079f;

constexpr auto GRID_OBJ = "grid";
constexpr auto BG_PLANE_OBJ = "bgPlane";
constexpr auto TRANSFORM_HANDLE = "transformHandle";
constexpr auto CAMERA_AXIS_HANDLE = "cameraAxisHandle";

constexpr auto TEXTURE_DIR = "resources/textures/";
constexpr auto FONT_DIR = "resources/fonts/";
constexpr auto MODELS_DIR = "resources/models/";
constexpr auto SHADER_DIR = "resources/shaders/";

constexpr auto CONFIG_JSON = "configs/config.json";
constexpr auto HOTKEYS_JSON = "configs/hotkeys.json";
constexpr auto MATS_JSON = "configs/materials.json";
constexpr auto SHADERS_JSON = "configs/shaders.json";
constexpr auto APP_NAME = "BlankMat";
constexpr auto ICON = "icon256.png";