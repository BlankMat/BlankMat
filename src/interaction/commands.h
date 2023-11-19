#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include "windows/window.h"
#include "interaction/runFunctionCommand.h"
#include "interaction/runToggleFunctionCommand.h"
#include "files/sceneIO.h"

class Commands
{
public:
	static void InitializeCommands(Window* appWindow)
	{
		Input* input = appWindow->GetInput();
		State* state = appWindow->GetState();
		Scene* scene = appWindow->GetScene();
		SceneIO* sIO = appWindow->GetSceneIO();
		GLFWwindow* window = appWindow->GetWindow();

		input->AddCommand("CAMERA_MOVE_F", new RunFunctionCommand("CAMERA_MOVE_F", nullptr));
		input->AddCommand("CAMERA_MOVE_B", new RunFunctionCommand("CAMERA_MOVE_B", nullptr));
		input->AddCommand("CAMERA_MOVE_L", new RunFunctionCommand("CAMERA_MOVE_L", nullptr));
		input->AddCommand("CAMERA_MOVE_R", new RunFunctionCommand("CAMERA_MOVE_R", nullptr));
		input->AddCommand("CAMERA_MOVE_U", new RunFunctionCommand("CAMERA_MOVE_U", nullptr));
		input->AddCommand("CAMERA_MOVE_D", new RunFunctionCommand("CAMERA_MOVE_D", nullptr));

		input->AddCommand("CAMERA_ROT_U", new RunFunctionCommand("CAMERA_ROT_U", nullptr));
		input->AddCommand("CAMERA_ROT_D", new RunFunctionCommand("CAMERA_ROT_D", nullptr));
		input->AddCommand("CAMERA_ROT_L", new RunFunctionCommand("CAMERA_ROT_L", nullptr));
		input->AddCommand("CAMERA_ROT_R", new RunFunctionCommand("CAMERA_ROT_R", nullptr));

		input->AddCommand("CAMERA_ROT_M", new RunFunctionCommand("CAMERA_ROT_M", std::bind(&Input::SetMouseInputMode, input, window, MouseInputMode::ROTATE)));
		input->AddCommand("CAMERA_ROT_M_RELEASE", new RunFunctionCommand("CAMERA_ROT_M_RELEASE", std::bind(&Input::SetMouseInputMode, input, window, MouseInputMode::DEFAULT)));
		input->AddCommand("CAMERA_MOVE_M", new RunFunctionCommand("CAMERA_MOVE_M", std::bind(&Input::SetMouseInputMode, input, window, MouseInputMode::MOVE)));
		input->AddCommand("CAMERA_MOVE_M_RELEASE", new RunFunctionCommand("CAMERA_MOVE_M_RELEASE", std::bind(&Input::SetMouseInputMode, input, window, MouseInputMode::DEFAULT)));
		input->AddCommand("CAMERA_ZOOM_M", new RunFunctionCommand("CAMERA_ZOOM_M", std::bind(&Input::SetMouseInputMode, input, window, MouseInputMode::ZOOM)));
		input->AddCommand("CAMERA_ZOOM_M_RELEASE", new RunFunctionCommand("CAMERA_ZOOM_M_RELEASE", std::bind(&Input::SetMouseInputMode, input, window, MouseInputMode::DEFAULT)));

		input->AddCommand("CAMERA_FOCUS", new RunFunctionCommand("CAMERA_FOCUS", std::bind(&Scene::FocusCamera, scene)));
		input->AddCommand("TOGGLE_VISIBILITY", new RunToggleFunctionCommand("TOGGLE_VISIBILITY", std::bind(&Selection::ToggleSelectionVisibility, state->GetSel())));

		input->AddCommand("SELECT_MODE", new RunFunctionCommand("SELECT_MODE", std::bind(&Selection::SetTool, state->GetSel(), Tool::SELECT)));
		input->AddCommand("MOVE_MODE", new RunFunctionCommand("MOVE_MODE", std::bind(&Selection::SetTool, state->GetSel(), Tool::MOVE)));
		input->AddCommand("ROTATE_MODE", new RunFunctionCommand("ROTATE_MODE", std::bind(&Selection::SetTool, state->GetSel(), Tool::ROTATE)));
		input->AddCommand("SCALE_MODE", new RunFunctionCommand("SCALE_MODE", std::bind(&Selection::SetTool, state->GetSel(), Tool::SCALE)));

		input->AddCommand("MESH_MODE", new RunFunctionCommand("MESH_MODE", std::bind(&Selection::SetSelMode, state->GetSel(), SelMode::MESH)));
		input->AddCommand("FACE_MODE", new RunFunctionCommand("FACE_MODE", std::bind(&Selection::SetSelMode, state->GetSel(), SelMode::FACE)));
		input->AddCommand("VERTEX_MODE", new RunFunctionCommand("VERTEX_MODE", std::bind(&Selection::SetSelMode, state->GetSel(), SelMode::VERT)));
		input->AddCommand("TOGGLE_CAMERA_ROT_MODE", new RunFunctionCommand("TOGGLE_CAMERA_ROT_MODE", std::bind(&Scene::ToggleCameraRotationMode, scene)));
		input->AddCommand("TOGGLE_CAMERA_PERSPECTIVE", new RunFunctionCommand("TOGGLE_CAMERA_PERSPECTIVE", std::bind(&Scene::TogglePerspective, scene)));

		input->AddCommand("LIGHTING_SHADED", new RunFunctionCommand("LIGHTING_SHADED", nullptr)); // TODO: Implement shading
		input->AddCommand("LIGHTING_TEXTURED", new RunFunctionCommand("LIGHTING_TEXTURED", nullptr)); // TODO: Implement shading
		input->AddCommand("LIGHTING_LIGHTS", new RunFunctionCommand("LIGHTING_LIGHTS", nullptr)); // TODO: Implement shading
		input->AddCommand("LIGHTING_WIREFRAME", new RunFunctionCommand("LIGHTING_WIREFRAME", nullptr)); // TODO: Implement shading
		input->AddCommand("TOGGLE_SHADOWS", new RunToggleFunctionCommand("TOGGLE_SHADOWS", std::bind(&State::ToggleShadows, state)));
		input->AddCommand("TOGGLE_DEFAULT_MAT", new RunToggleFunctionCommand("TOGGLE_DEFAULT_MAT", std::bind(&State::ToggleDefaultMat, state)));
		input->AddCommand("RENDER", new RunFunctionCommand("RENDER", nullptr)); // TODO: Implement render

		input->AddCommand("TOGGLE_DIFFUSE_MAP", new RunToggleFunctionCommand("TOGGLE_DIFFUSE_MAP", std::bind(&State::ToggleDiffuseMap, state)));
		input->AddCommand("TOGGLE_AMBIENT_MAP", new RunToggleFunctionCommand("TOGGLE_AMBIENT_MAP", std::bind(&State::ToggleAmbientMap, state)));
		input->AddCommand("TOGGLE_SPECULAR_MAP", new RunToggleFunctionCommand("TOGGLE_SPECULAR_MAP", std::bind(&State::ToggleSpecularMap, state)));
		input->AddCommand("TOGGLE_NORMAL_MAP", new RunToggleFunctionCommand("TOGGLE_NORMAL_MAP", std::bind(&State::ToggleNormalMap, state)));
		input->AddCommand("TOGGLE_HEIGHT_MAP", new RunToggleFunctionCommand("TOGGLE_HEIGHT_MAP", std::bind(&State::ToggleHeightMap, state)));
		input->AddCommand("TOGGLE_ALPHA_MAP", new RunToggleFunctionCommand("TOGGLE_ALPHA_MAP", std::bind(&State::ToggleAlphaMap, state)));

		input->AddCommand("TOGGLE_UI", new RunFunctionCommand("TOGGLE_UI", std::bind(&State::ToggleDisplayGUI, state)));
		input->AddCommand("TOGGLE_GRID", new RunFunctionCommand("TOGGLE_GRID", std::bind(&State::ToggleDisplayGrid, state)));

		input->AddCommand("NEW_SCENE", new RunFunctionCommand("NEW_SCENE", std::bind(&SceneIO::NewScene, sIO)));
		input->AddCommand("OPEN_SCENE", new RunFunctionCommand("OPEN_SCENE", std::bind(&SceneIO::OpenScene, sIO)));
		input->AddCommand("SAVE_SCENE", new RunFunctionCommand("SAVE_SCENE", std::bind(&SceneIO::SaveScene, sIO)));
		input->AddCommand("SAVE_SCENE_AS", new RunFunctionCommand("SAVE_SCENE_AS", std::bind(&SceneIO::SaveSceneAs, sIO)));
		input->AddCommand("SAVE_SCENE_INCREMENT", new RunFunctionCommand("SAVE_SCENE_INCREMENT", std::bind(&SceneIO::SaveSceneIncrement, sIO)));
		input->AddCommand("IMPORT", new RunFunctionCommand("IMPORT", std::bind(&SceneIO::Import, sIO)));
		input->AddCommand("EXPORT", new RunFunctionCommand("EXPORT", std::bind(&SceneIO::Export, sIO)));
		input->AddCommand("QUIT", new RunFunctionCommand("QUIT", std::bind(&Window::Quit, appWindow)));

		input->AddCommand("UNDO", new RunFunctionCommand("UNDO", std::bind(&ActionStack::Undo, state->GetActionStack())));
		input->AddCommand("REDO", new RunFunctionCommand("REDO", std::bind(&ActionStack::Redo, state->GetActionStack())));
		input->AddCommand("REPEAT", new RunFunctionCommand("REPEAT", nullptr)); // TODO: Implement repeat
		input->AddCommand("COPY", new RunFunctionCommand("COPY", nullptr)); // Copy works by default
		input->AddCommand("CUT", new RunFunctionCommand("CUT", nullptr)); // Cut works by default
		input->AddCommand("PASTE", new RunFunctionCommand("PASTE", nullptr)); // Paste works by default
	}
};