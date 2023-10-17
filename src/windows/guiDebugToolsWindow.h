#pragma once
#include "iGUIWindow.h"
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
					std::string itemName = iter->first;
					bool isSelected = itemName == curShader;
					if (ImGui::Selectable(itemName.c_str(), &isSelected))
						curShader = itemName;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
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
			ImGui::Checkbox("Enable Diffuse Map", &mState->enableDiffuseMap);
			ImGui::Checkbox("Enable Ambient Map", &mState->enableAmbientMap);
			ImGui::Checkbox("Enable Specular Map", &mState->enableSpecularMap);
			ImGui::Checkbox("Enable Normal Map", &mState->enableNormalMap);
			ImGui::Checkbox("Enable Height Map", &mState->enableHeightMap);
			ImGui::Checkbox("Enable Alpha Map", &mState->enableAlphaMap);

			// Camera settings
			ImGui::Text("Camera Settings");
			Camera* cam = mScene->GetCamera();

			// Camera position
			glm::vec3 camPos = cam->GetPos();
			float camPosInput[3] = { camPos.x, camPos.y, camPos.z };
			ImGui::InputFloat3("Camera Position", camPosInput);
			cam->SetPos(Vec3FromFloats(camPosInput));

			// Camera lookat
			glm::vec3 camTarget = cam->GetLookAt();
			float camLookInput[3] = { camTarget.x, camTarget.y, camTarget.z };
			ImGui::InputFloat3("Camera Target", camLookInput);
			cam->LookAt(Vec3FromFloats(camLookInput));

			// Camera size
			float camSize = cam->GetOrthSize();
			ImGui::InputFloat("Camera Size", &camSize);
			cam->SetOrthSize(camSize);

			// Camera FOV
			float camFOV = cam->GetFOV();
			ImGui::InputFloat("Camera FOV", &camFOV);
			cam->SetFOV(camFOV);

			// Camera near clip
			float camNearClip = cam->GetNearClip();
			ImGui::InputFloat("Camera Near Clip", &camNearClip);
			cam->SetNearClip(camNearClip);

			// Camera far clip
			float camFarClip = cam->GetFarClip();
			ImGui::InputFloat("Camera Far Clip", &camFarClip);
			cam->SetFarClip(camFarClip);

			// Perspective
			bool camPerspective = cam->IsPerspective();
			ImGui::Checkbox("Perspective", &camPerspective);
			cam->SetPerspective(camPerspective);

			// Wireframe
			bool camWireframe = cam->IsWireframe();
			ImGui::Checkbox("Wireframe", &camWireframe);
			cam->SetWireframe(camWireframe);

			// Debug settings
			ImGui::Text("Debug settings");
			ImGui::Checkbox("Disco Light", &mState->isDiscoLight);
			ImGui::Checkbox("Rotating Light", &mState->isRotatingLight);
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