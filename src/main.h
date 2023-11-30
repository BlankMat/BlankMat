#pragma once
#include "timer.h"
#include "interaction/input.h"
#include "interaction/actionStack.h"
#include "interaction/commands.h"

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
#include "windows/guiToolModeWindow.h"

#include "windows/guiLightViewer.h"
#include "windows/guiMaterialViewer.h"

#include "windows/guiActionList.h"
#include "windows/guiMaterialEditor.h"

#include "rendering/shader.h"
#include "rendering/scene.h"

#include "primitives/pPlane.h"
#include "primitives/pGrid.h"
#include "primitives/pLightCube.h"
#include "primitives/pHandle.h"
#include "primitives/vCube.h"
#include "primitives/vPlane.h"

#include <thread>

constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

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
/// <param name="state">Global state</param>
/// <param name="scene">Scene reference</param>
/// <param name="config">Config file</param>
void LoadGUIs(Window* window, State* state, Scene* scene, Input* input, Config* config);

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
/// <param name="state">Global state</param>
/// <param name="defaultMat">Default material</param>
/// <param name="defaultCubes">Whether to draw sample cubes or not</param>
/// <param name="cameraConfig">Default camera</param>
/// <param name="lightConfig">Default light</param>
void LoadDefaultScene(Scene* scene, State* state, Material* defaultMat, bool defaultCubes, Config* cameraConfig, Config* lightConfig);

/// <summary>
/// Handles quitting from the window
/// </summary>
/// <param name="window">Window to quit</param>
/// <param name="state">Global state</param>
/// <returns>Whether the application should be quit</returns>
bool HandleWindowQuit(Window* window, State* state);
