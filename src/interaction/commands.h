#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "windows/window.h"
#include "interaction/runFunctionCommand.h"
#include "files/fileOperations.h"

class Commands
{
public:
	static void InitializeCommands(Window* window, State* state)
	{
		Input* input = window->GetInput();

		input->AddCommand("CAMERA_MOVE_F", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_MOVE_B", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_MOVE_L", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_MOVE_R", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_MOVE_U", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_MOVE_D", new RunFunctionCommand(nullptr));

		input->AddCommand("CAMERA_ROT_U", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_ROT_D", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_ROT_L", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_ROT_R", new RunFunctionCommand(nullptr));

		input->AddCommand("CAMERA_ROT_M", new RunFunctionCommand(nullptr));
		input->AddCommand("CAMERA_MOVE_M", new RunFunctionCommand(nullptr));

		input->AddCommand("CAMERA_FOCUS", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_VISIBILITY", new RunFunctionCommand(nullptr));

		input->AddCommand("SELECT_MODE", new RunFunctionCommand(nullptr));
		input->AddCommand("MOVE_MODE", new RunFunctionCommand(nullptr));
		input->AddCommand("ROTATE_MODE", new RunFunctionCommand(nullptr));
		input->AddCommand("SCALE_MODE", new RunFunctionCommand(nullptr));

		input->AddCommand("MESH_MODE", new RunFunctionCommand(nullptr));
		input->AddCommand("FACE_MODE", new RunFunctionCommand(nullptr));
		input->AddCommand("VERTEX_MODE", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_CAMERA_ROT_MODE", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_CAMERA_PERSPECTIVE", new RunFunctionCommand(nullptr));

		input->AddCommand("LIGHTING_SHADED", new RunFunctionCommand(nullptr));
		input->AddCommand("LIGHTING_TEXTURED", new RunFunctionCommand(nullptr));
		input->AddCommand("LIGHTING_LIGHTS", new RunFunctionCommand(nullptr));
		input->AddCommand("LIGHTING_WIREFRAME", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_SHADOWS", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_DEFAULT_MAT", new RunFunctionCommand(nullptr));
		input->AddCommand("RENDER", new RunFunctionCommand(nullptr));

		input->AddCommand("TOGGLE_DIFFUSE_MAP", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_AMBIENT_MAP", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_SPECULAR_MAP", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_NORMAL_MAP", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_HEIGHT_MAP", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_ALPHA_MAP", new RunFunctionCommand(nullptr));

		input->AddCommand("TOGGLE_UI", new RunFunctionCommand(nullptr));
		input->AddCommand("TOGGLE_GRID", new RunFunctionCommand(nullptr));

		input->AddCommand("NEW_SCENE", new RunFunctionCommand([&]() { window->GetFIO()->NewScene(); }));
		input->AddCommand("OPEN_SCENE", new RunFunctionCommand([&]() { window->GetFIO()->OpenScene(); }));
		input->AddCommand("SAVE_SCENE", new RunFunctionCommand([&]() { window->GetFIO()->SaveScene(); }));
		input->AddCommand("SAVE_SCENE_AS", new RunFunctionCommand([&]() { window->GetFIO()->SaveSceneAs(); }));
		input->AddCommand("SAVE_SCENE_INCREMENT", new RunFunctionCommand([&]() { window->GetFIO()->SaveSceneIncrement(); }));
		input->AddCommand("IMPORT", new RunFunctionCommand([&]() { window->GetFIO()->Import(); }));
		input->AddCommand("EXPORT", new RunFunctionCommand([&]() { window->GetFIO()->Export(); }));
		input->AddCommand("QUIT", new RunFunctionCommand([&]() { window->Quit(); }));

		input->AddCommand("UNDO", new RunFunctionCommand(nullptr));
		input->AddCommand("REDO", new RunFunctionCommand(nullptr));
		input->AddCommand("REPEAT", new RunFunctionCommand(nullptr));
		input->AddCommand("COPY", new RunFunctionCommand(nullptr));
		input->AddCommand("CUT", new RunFunctionCommand(nullptr));
		input->AddCommand("PASTE", new RunFunctionCommand(nullptr));
	}
};