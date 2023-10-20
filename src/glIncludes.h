#pragma once
#include <imgui/imgui.h>
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
constexpr float PI = 3.141592f;
constexpr float HALF_PI = 1.57079f;

#define VERT_SHADER_SIZE 12

#define GRID_OBJ "grid"
#define BG_PLANE_OBJ "bgPlane"
#define TRANSFORM_HANDLE "transformHandle"
#define CAMERA_AXIS_HANDLE "cameraAxisHandle"

#define TEXTURE_DIR "resources/textures/"
#define FONT_DIR "resources/fonts/"
#define MODELS_DIR "resources/models/"
#define SHADER_DIR "resources/shaders/"

#define CONFIG_JSON "configs/config.json"
#define MATS_JSON "configs/materials.json"
#define SHADERS_JSON "configs/shaders.json"
#define APP_NAME "BlankMat"
#define ICON "icon256.png"