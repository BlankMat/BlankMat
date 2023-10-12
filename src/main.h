#pragma once
#include "input.h"

#include "tools/state.h"

#include "files/config.h"
#include "files/fileSystem.h"
#include "files/configReader.h"

#include "windows/window.h"
#include "windows/guiDebugToolsWindow.h"
#include "windows/guiMenuBarWindow.h"

#include "rendering/shader.h"
#include "rendering/scene.h"

#include "primitives/pPlane.h"
#include "primitives/pGrid.h"
#include "primitives/pLightCube.h"
#include "primitives/pHandle.h"

#define CONFIG_JSON "configs/config.json"
#define MATS_JSON "configs/materials.json"
#define MODELS_DIR "resources/models/"
#define APP_NAME "BlankMat"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main();
void OpenGLDraw(Window* window, IScene* scene, State* state);