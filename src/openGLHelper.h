#pragma once
#include "fileHelper.h"
#include "scene.h"
#include "model.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

struct ProgramIDs
{
	unsigned int shaderProgram;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

// OpenGL helper functions
GLFWwindow* OpenGLInitWindow(int width, int height, std::string name);
void OpenGLInitBuffers(ProgramIDs* ids, int vertsSize, float* vertices, int indicesSize, unsigned int* indices);
void OpenGLDraw(Model* model, Scene* scene, Selection* sel, ProgramIDs* ids);
void OpenGLCleanup(ProgramIDs* ids);
void OpenGLEnableWireframe(bool enable);

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);