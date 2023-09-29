#pragma once
#include "scene.h"
#include "options.h"
#include "selection.h"
#include "rayTracer.h"

#define KEY1_PRESS (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
#define KEY2_PRESS (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
#define KEY3_PRESS (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
#define KEY4_PRESS (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
#define KEY5_PRESS (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
#define C_PRESS (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
#define Q_PRESS (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
#define E_PRESS (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
#define R_PRESS (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
#define T_PRESS (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
#define S_PRESS (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
#define F_PRESS (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
#define ESC_PRESS (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
#define WASDZX_PRESS (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
#define ARROW_PRESS (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
#define DELETE_PRESS (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
#define SHIFT_PRESS (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
#define ALT_PRESS (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
#define CTRL_PRESS (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
#define MOUSE_PRESS (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
#define LEFT_MOUSE_PRESS (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
#define RIGHT_MOUSE_PRESS (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)

// Input lock structure to separate presses from holding keyes
struct InputLocks {
	bool lockQ = false;			// Select
	bool lockE = false;			// Scale
	bool lockR = false;			// Rotate
	bool lockT = false;			// Translate
	bool lockCtrlE = false;		// Extrude

	bool lockF = false;			// Focus
	bool lockCtrlC = false;		// Center pivot
	bool lockCtrlR = false;		// Render
	bool lockCtrlT = false;		// Triangulate
	bool lockCtrlS = false;		// Save

	bool lockDel = false;		// Delete
	bool lockLeftMouse = false;
	bool lockRightMouse = false;

	bool lockKey1 = false;
	bool lockKey2 = false;
	bool lockKey3 = false;
	bool lockKey4 = false;
	bool lockKey5 = false;

	bool reselect = false;
	bool rerender = false;

	void ClearLocks();
	void LockTool(Tool _tool);
};

bool ProcessInput(GLFWwindow* window, Scene* scene, Selection* sel, InputLocks* locks, Options* options, SpeedConsts* speeds, float deltaTime, int* prevX, int* prevY);
glm::vec3 GetWASDZX(GLFWwindow* window);
glm::vec3 GetArrow(GLFWwindow* window);