#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
constexpr float PI = 3.141592f;
constexpr float HALF_PI = 1.57079f;

constexpr size_t VERT_SHADER_SIZE = 12;

constexpr char* GRID_OBJ = "grid";
constexpr char* BG_PLANE_OBJ = "bgPlane";
constexpr char* TRANSFORM_HANDLE = "transformHandle";
constexpr char* CAMERA_AXIS_HANDLE = "cameraAxisHandle";

constexpr char* TEXTURE_DIR = "resources/textures/";
constexpr char* FONT_DIR = "resources/fonts/";
constexpr char* MODELS_DIR = "resources/models/";
constexpr char* SHADER_DIR = "resources/shaders/";

constexpr char* CONFIG_JSON = "configs/config.json";
constexpr char* MATS_JSON = "configs/materials.json";
constexpr char* SHADERS_JSON = "configs/shaders.json";
constexpr char* APP_NAME = "BlankMat";
constexpr char* ICON = "icon256.png";