#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include "windows/window.h"
#include "interaction/runFunctionCommand.h"
#include "files/sceneIO.h"

class Commands
{
public:
	static void InitializeCommands(Window* window, State* state, Scene* scene)
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

		input->AddCommand("CAMERA_FOCUS", new RunFunctionCommand([&]() {
			std::cout << "Camera Focus command" << std::endl;
			/*
			IEntity* sel = state->GetSel()->GetSelectedEntity();
			scene->GetCamera()->LookAt(sel != nullptr ? sel->GetPos() : glm::vec3(0.0f));
			*/
		}));
		input->AddCommand("TOGGLE_VISIBILITY", new RunFunctionCommand([&]() { state->GetSel()->ToggleSelectionVisibility(); }));

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
		input->AddCommand("TOGGLE_SHADOWS", new RunFunctionCommand([&]() { state->ToggleShadows(); }));
		input->AddCommand("TOGGLE_DEFAULT_MAT", new RunFunctionCommand([&]() { state->ToggleDefaultMat(); }));
		input->AddCommand("RENDER", new RunFunctionCommand(nullptr));

		input->AddCommand("TOGGLE_DIFFUSE_MAP", new RunFunctionCommand([&]() { state->ToggleDiffuseMap(); }));
		input->AddCommand("TOGGLE_AMBIENT_MAP", new RunFunctionCommand([&]() { state->ToggleAmbientMap(); }));
		input->AddCommand("TOGGLE_SPECULAR_MAP", new RunFunctionCommand([&]() { state->ToggleSpecularMap(); }));
		input->AddCommand("TOGGLE_NORMAL_MAP", new RunFunctionCommand([&]() { state->ToggleNormalMap(); }));
		input->AddCommand("TOGGLE_HEIGHT_MAP", new RunFunctionCommand([&]() { state->ToggleHeightMap(); }));
		input->AddCommand("TOGGLE_ALPHA_MAP", new RunFunctionCommand([&]() { state->ToggleAlphaMap(); }));

		input->AddCommand("TOGGLE_UI", new RunFunctionCommand([&]() { state->ToggleDisplayGUI(); }));
		input->AddCommand("TOGGLE_GRID", new RunFunctionCommand([&]() { state->ToggleDisplayGrid(); }));

		input->AddCommand("NEW_SCENE", new RunFunctionCommand([&]() { window->GetSceneIO()->NewScene(); }));
		input->AddCommand("OPEN_SCENE", new RunFunctionCommand([&]() { window->GetSceneIO()->OpenScene(); }));
		input->AddCommand("SAVE_SCENE", new RunFunctionCommand([&]() { window->GetSceneIO()->SaveScene(); }));
		input->AddCommand("SAVE_SCENE_AS", new RunFunctionCommand([&]() { window->GetSceneIO()->SaveSceneAs(); }));
		input->AddCommand("SAVE_SCENE_INCREMENT", new RunFunctionCommand([&]() { window->GetSceneIO()->SaveSceneIncrement(); }));
		input->AddCommand("IMPORT", new RunFunctionCommand([&]() { window->GetSceneIO()->Import(); }));
		input->AddCommand("EXPORT", new RunFunctionCommand([&]() { window->GetSceneIO()->Export(); }));
		input->AddCommand("QUIT", new RunFunctionCommand([&]() { window->Quit(); }));

		input->AddCommand("UNDO", new RunFunctionCommand(nullptr));
		input->AddCommand("REDO", new RunFunctionCommand(nullptr));
		input->AddCommand("REPEAT", new RunFunctionCommand(nullptr));
		input->AddCommand("COPY", new RunFunctionCommand(nullptr));
		input->AddCommand("CUT", new RunFunctionCommand(nullptr));
		input->AddCommand("PASTE", new RunFunctionCommand(nullptr));
	}
};