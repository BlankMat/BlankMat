#pragma once
#include "fileSystem.h"
#include "options.h"
#include "window.h"
#include "shader.h"
#include "lightCube.h"
#include "transformHandle.h"
#include "model.h"
#include "input.h"

#define CONFIG_FILE "config.txt"
#define MODELS_DIR "resources/models/"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

void OpenGLDraw(Window* window, Scene* scene, Selection* sel);