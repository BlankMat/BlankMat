#pragma once
#include "fileHelper.h"
#include "scene.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

struct ProgramIDs
{
	unsigned int shaderProgram;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int matrixID;
	unsigned int modelID;
	unsigned int normalModelID;
	unsigned int lightPosID;
	unsigned int lightColorID;
	unsigned int viewPosID;
	unsigned int ambientStrengthID;
	unsigned int specularStrengthID;
	unsigned int winScaleID;

	void GetUniformIDs();
};

// OpenGL helper functions
GLFWwindow* OpenGLInitWindow(int width, int height, std::string name);
void OpenGLInitBuffers(ProgramIDs* ids, int vertsSize, float* vertices, int indicesSize, unsigned int* indices);
void OpenGLDraw(Scene* scene, Selection* sel, ProgramIDs* ids, int indicesSize, unsigned int* indices);
void OpenGLCleanup(ProgramIDs* ids);
void OpenGLEnableWireframe(bool enable);

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);