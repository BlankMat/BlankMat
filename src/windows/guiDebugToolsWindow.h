#pragma once
#include "iGUIWindow.h"
#include "guiWindowUtils.h"
#include "selection.h"
#include "utils.h"
#include "rendering/scene.h"
#include "rendering/camera.h"
#include "tools/state.h"

class GUIDebugToolsWindow : public IGUIWindow
{
protected:
	State* mState;
	Scene* mScene;
public:
	void Draw() override
	{
		// Don't draw disabled GUI
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Debug Tools", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Shading settings
			ImGui::Text("Shading");
			// Select shader
			std::unordered_map<std::string, Shader*> shaders = mScene->GetShaderList();
			std::string curShader = mScene->GetCurShader();
			if (ImGui::BeginListBox("Shader"))
			{
				for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
				{
					GUIWindowUtils::Selectable(iter->first, curShader, iter->first);
				}
				ImGui::EndListBox();

				// Only change the shader if it actually changed
				if (mScene->GetCurShader() != curShader)
				{
					mScene->UseShader(curShader);
					mScene->GetRootNode()->SetShader(mScene->GetShader(curShader));
				}
			}

			// Choose parts of materials
			mState->enableDiffuseMap = GUIWindowUtils::Checkbox("Enable Diffuse Map", mState->enableDiffuseMap);
			mState->enableAmbientMap = GUIWindowUtils::Checkbox("Enable Ambient Map", mState->enableAmbientMap);
			mState->enableSpecularMap = GUIWindowUtils::Checkbox("Enable Specular Map", mState->enableSpecularMap);
			mState->enableNormalMap = GUIWindowUtils::Checkbox("Enable Normal Map", mState->enableNormalMap);
			mState->enableHeightMap = GUIWindowUtils::Checkbox("Enable Height Map", mState->enableHeightMap);
			mState->enableAlphaMap = GUIWindowUtils::Checkbox("Enable Alpha Map", mState->enableAlphaMap);

			// Camera settings
			ImGui::Text("Camera Settings");
			Camera* cam = mScene->GetCamera();
			cam->SetPos(
				GUIWindowUtils::InputVec3("Position", cam->GetPos()));
			cam->LookAt(
				GUIWindowUtils::InputVec3("Target", cam->GetLookAt()));
			cam->SetOrthSize(
				GUIWindowUtils::InputFloat("Size", cam->GetOrthSize()));
			cam->SetFOV(
				GUIWindowUtils::InputFloat("FOV", cam->GetFOV()));
			cam->SetNearClip(
				GUIWindowUtils::InputFloat("Near Clip", cam->GetNearClip()));
			cam->SetFarClip(
				GUIWindowUtils::InputFloat("Far Clip", cam->GetFarClip()));
			cam->SetPerspective(
				GUIWindowUtils::Checkbox("Perspective", cam->IsPerspective()));
			cam->SetWireframe(
				GUIWindowUtils::Checkbox("Wireframe", cam->IsWireframe()));

			// Debug settings
			ImGui::Text("Debug settings");
			mState->isDiscoLight = GUIWindowUtils::Checkbox("Disco Light", mState->isDiscoLight);
			mState->isRotatingLight = GUIWindowUtils::Checkbox("Rotating Light", mState->isRotatingLight);
		}
		ImGui::End();
	}

	GUIDebugToolsWindow(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::DEBUG_TOOLS;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};