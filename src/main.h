#pragma once
#include "fileSystem.h"
#include "options.h"
#include "window.h"
#include "shader.h"
#include "lightCube.h"
#include "transformHandle.h"
#include "plane.h"
#include "model.h"
#include "input.h"

#define CONFIG_FILE "config.txt"
#define MODELS_DIR "resources/models/"
#define APP_NAME "BlankMat"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

void OpenGLDraw(Window* window, Scene* scene, Selection* sel, Options* options);