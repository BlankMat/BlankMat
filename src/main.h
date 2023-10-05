#pragma once
#include "input.h"

#include "files/options.h"
#include "files/fileSystem.h"
#include "files/configReader.h"

#include "windows/window.h"
#include "windows/guiDebugToolsWindow.h"

#include "rendering/shader.h"
#include "rendering/model.h"
#include "rendering/modelScene.h"

#include "primitives/pplane.h"
#include "primitives/pgrid.h"
#include "primitives/pLightCube.h"
#include "primitives/pHandle.h"

#define CONFIG_FILE "configs/config.txt"
#define CONFIG_JSON "configs/config.json"
#define MODELS_DIR "resources/models/"
#define APP_NAME "BlankMat"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main();
void OpenGLDraw(Window* window, IScene* scene, Selection* sel, Options* options);