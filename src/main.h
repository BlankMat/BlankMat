#pragma once
#include "fileSystem.h"
#include "options.h"
#include "window.h"
#include "shader.h"
#include "model.h"
#include "modelScene.h"
#include "input.h"
#include "plane.h"
#include "grid.h"
#include "lightCube.h"
#include "transformHandle.h"

#define CONFIG_FILE "config.txt"
#define MODELS_DIR "resources/models/"
#define APP_NAME "BlankMat"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main();
void OpenGLDraw(Window* window, IScene* scene, Selection* sel, Options* options);