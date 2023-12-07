#pragma once
// PFD *MUST* be first import to prevent windows errors
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

constexpr float PI = 3.141592f;
constexpr float HALF_PI = 1.57079f;