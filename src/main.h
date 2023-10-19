#pragma once
#include "input.h"

#include "tools/state.h"

#include "files/config.h"
#include "files/fileSystem.h"
#include "files/configReader.h"

#include "windows/window.h"
#include "windows/guiDebugToolsWindow.h"
#include "windows/guiMenuBarWindow.h"
#include "windows/guiHierarchyWindow.h"
#include "windows/guiInspectorWindow.h"
#include "windows/guiToolbarWindow.h"

#include "windows/guiLightViewer.h"
#include "windows/guiMaterialViewer.h"

#include "windows/guiMaterialEditor.h"

#include "rendering/shader.h"
#include "rendering/scene.h"

#include "primitives/pPlane.h"
#include "primitives/pGrid.h"
#include "primitives/pLightCube.h"
#include "primitives/pHandle.h"
#include "primitives/vCube.h"
#include "primitives/vPlane.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main();
void OpenGLDraw(Window* window, IScene* scene, State* state);