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

// Main program loop
int main();

/// <summary>
/// Draws the current scene
/// </summary>
/// <param name="window">Window to draw to</param>
/// <param name="scene">Scene to draw</param>
/// <param name="state">Global state to use</param>
void OpenGLDraw(Window* window, State* state, Scene* scene);

/// <summary>
/// Opens all defined GUIs
/// </summary>
/// <param name="window">Window to load GUIs to</param>
/// <param name="state"></param>
/// <param name="scene"></param>
void LoadGUIs(Window* window, State* state, Scene* scene);

/// <summary>
/// Loads all defined shaders
/// </summary>
/// <param name="scene">Scene to load shaders to</param>
/// <param name="shaderConfig">Config to load shaders from</param>
void LoadShaders(Scene* scene, Config* shaderConfig);

/// <summary>
/// Loads the default testing scene
/// </summary>
/// <param name="scene">Scene to load to</param>
/// <param name="defaultMat">Default material</param>
/// <param name="defaultCubes">Whether to draw sample cubes or not</param>
/// <param name="cameraConfig">Default camera</param>
/// <param name="lightConfig">Default light</param>
void LoadDefaultScene(Scene* scene, Material* defaultMat, bool defaultCubes, Config* cameraConfig, Config* lightConfig);
