#pragma once
#include "blankMatConfig.h"
#include "timer.h"
#include "interaction/input.h"
#include "interaction/actionStack.h"
#include "interaction/commands.h"

#include "interaction/state.h"

#include "files/config.h"
#include "files/fileSystem.h"
#include "files/configReader.h"

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
int main(int argc, char* argv[]);

/// <summary>
/// Draws the current scene
/// </summary>
/// <param name="window">Window to draw to</param>
/// <param name="scene">Scene to draw</param>
/// <param name="state">Global state to use</param>
void OpenGLDraw(Window* window, State* state, Scene* scene);

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
