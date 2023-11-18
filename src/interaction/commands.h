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
	static void InitializeCommands(Window* window)
	{
		Input* input = window->GetInput();
		State* state = window->GetState();
		Scene* scene = window->GetScene();
		SceneIO* sIO = window->GetSceneIO();

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
		input->AddCommand("TOGGLE_VISIBILITY", new RunFunctionCommand(std::bind(&Selection::ToggleSelectionVisibility, state->GetSel())));

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
		input->AddCommand("TOGGLE_SHADOWS", new RunFunctionCommand(std::bind(&State::ToggleShadows, state)));
		input->AddCommand("TOGGLE_DEFAULT_MAT", new RunFunctionCommand(std::bind(&State::ToggleDefaultMat, state)));
		input->AddCommand("RENDER", new RunFunctionCommand(nullptr));

		input->AddCommand("TOGGLE_DIFFUSE_MAP", new RunFunctionCommand(std::bind(&State::ToggleDiffuseMap, state)));
		input->AddCommand("TOGGLE_AMBIENT_MAP", new RunFunctionCommand(std::bind(&State::ToggleAmbientMap, state)));
		input->AddCommand("TOGGLE_SPECULAR_MAP", new RunFunctionCommand(std::bind(&State::ToggleSpecularMap, state)));
		input->AddCommand("TOGGLE_NORMAL_MAP", new RunFunctionCommand(std::bind(&State::ToggleNormalMap, state)));
		input->AddCommand("TOGGLE_HEIGHT_MAP", new RunFunctionCommand(std::bind(&State::ToggleHeightMap, state)));
		input->AddCommand("TOGGLE_ALPHA_MAP", new RunFunctionCommand(std::bind(&State::ToggleAlphaMap, state)));

		input->AddCommand("TOGGLE_UI", new RunFunctionCommand(std::bind(&State::ToggleDisplayGUI, state)));
		input->AddCommand("TOGGLE_GRID", new RunFunctionCommand(std::bind(&State::ToggleDisplayGrid, state)));

		input->AddCommand("NEW_SCENE", new RunFunctionCommand(std::bind(&SceneIO::NewScene, sIO)));
		input->AddCommand("OPEN_SCENE", new RunFunctionCommand(std::bind(&SceneIO::OpenScene, sIO)));
		input->AddCommand("SAVE_SCENE", new RunFunctionCommand(std::bind(&SceneIO::SaveScene, sIO)));
		input->AddCommand("SAVE_SCENE_AS", new RunFunctionCommand(std::bind(&SceneIO::SaveSceneAs, sIO)));
		input->AddCommand("SAVE_SCENE_INCREMENT", new RunFunctionCommand(std::bind(&SceneIO::SaveSceneIncrement, sIO)));
		input->AddCommand("IMPORT", new RunFunctionCommand(std::bind(&SceneIO::Import, sIO)));
		input->AddCommand("EXPORT", new RunFunctionCommand(std::bind(&SceneIO::Export, sIO)));
		input->AddCommand("QUIT", new RunFunctionCommand(std::bind(&Window::Quit, window)));

		input->AddCommand("UNDO", new RunFunctionCommand(std::bind(&ActionStack::Undo, state->GetActionStack())));
		input->AddCommand("REDO", new RunFunctionCommand(std::bind(&ActionStack::Redo, state->GetActionStack())));
		input->AddCommand("REPEAT", new RunFunctionCommand(nullptr));
		input->AddCommand("COPY", new RunFunctionCommand(nullptr));
		input->AddCommand("CUT", new RunFunctionCommand(nullptr));
		input->AddCommand("PASTE", new RunFunctionCommand(nullptr));
	}
};