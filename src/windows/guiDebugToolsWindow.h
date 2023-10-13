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

		if (ImGui::Begin("Debug Tools"))
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
				mScene->UseShader(curShader);
				mScene->GetRootNode()->SetShader(mScene->GetShader(curShader));
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

			// Light settings
			ImGui::Text("Light Settings");
			Light* light = mScene->GetLight();

			// Light position
			glm::vec3 lightPos = light->GetOffset();
			float lightPosInput[3] = { lightPos.x, lightPos.y, lightPos.z };
			ImGui::InputFloat3("Light Position", lightPosInput);
			light->SetOffset(Vec3FromFloats(lightPosInput));

			// Light color
			glm::vec3 lightColor = light->GetBaseColor();
			float lightColorInput[3] = { lightColor.x, lightColor.y, lightColor.z };
			ImGui::ColorPicker3("Light Color", lightColorInput);
			light->SetBaseColor(Vec3FromFloats(lightColorInput));

			// Light KD
			float lightKD = light->GetKD();
			ImGui::InputFloat("Light Diffuse", &lightKD);
			light->SetKD(lightKD);

			// Light KA
			float lightKA = light->GetKA();
			ImGui::InputFloat("Light Ambient", &lightKA);
			light->SetKA(lightKA);

			// Light KS
			float lightKS = light->GetKS();
			ImGui::InputFloat("Light Specular", &lightKS);
			light->SetKS(lightKS);

			// Light gamma
			bool lightGamma = light->GetGamma();
			ImGui::Checkbox("Light Gamma", &lightGamma);
			light->SetGamma(lightGamma);

			// Model settings
			Node* model = mScene->GetRootNode();
			ImGui::Text("Model Settings");

			// Model position
			glm::vec3 modelPos = model->GetPos();
			float modelPosInput[3] = { modelPos.x, modelPos.y, modelPos.z };
			ImGui::InputFloat3("Model Position", modelPosInput);
			model->SetPos(Vec3FromFloats(modelPosInput));

			// Model rotation
			glm::vec3 modelRot = model->GetRot();
			float modelRotInput[3] = { modelRot.x, modelRot.y, modelRot.z };
			ImGui::InputFloat3("Model Rotation", modelRotInput);
			model->SetRot(Vec3FromFloats(modelRotInput));

			// Model scale
			glm::vec3 modelScale = model->GetScale();
			float modelScaleInput[3] = { modelScale.x, modelScale.y, modelScale.z };
			ImGui::InputFloat3("Model Scale", modelScaleInput);
			model->SetScale(Vec3FromFloats(modelScaleInput));

			// Brick Wall settings
			IEntity* wall = mScene->GetEntity("brickwall");
			ImGui::Text("Brick Wall Settings");

			// Wall position
			glm::vec3 wallPos = wall->GetPos();
			float wallPosInput[3] = { wallPos.x, wallPos.y, wallPos.z };
			ImGui::InputFloat3("Wall Position", wallPosInput);
			wall->SetPos(Vec3FromFloats(wallPosInput));

			// Wall rotation
			glm::vec3 wallRot = wall->GetRot();
			float wallRotInput[3] = { wallRot.x, wallRot.y, wallRot.z };
			ImGui::InputFloat3("Wall Rotation", wallRotInput);
			wall->SetRot(Vec3FromFloats(wallRotInput));

			// Wall scale
			glm::vec3 wallScale = wall->GetScale();
			float wallScaleInput[3] = { wallScale.x, wallScale.y, wallScale.z };
			ImGui::InputFloat3("Wall Scale", wallScaleInput);
			wall->SetScale(Vec3FromFloats(wallScaleInput));

			Shader* wallShader = wall->GetShader();
			if (ImGui::BeginListBox("Wall Shader"))
			{
				for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
				{
					std::string itemName = iter->first;
					unsigned int itemID = iter->second->ID;
					bool isSelected = itemID == wallShader->ID;
					if (ImGui::Selectable(itemName.c_str(), &isSelected))
						wallShader = iter->second;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
				wall->SetShader(wallShader);
			}

			// Debug settings
			ImGui::Text("Debug settings");
			ImGui::Checkbox("Disco Light", &mState->isDiscoLight);
			ImGui::Checkbox("Rotating Light", &mState->isRotatingLight);
			ImGui::End();
		}
	}

	GUIDebugToolsWindow(State* state, Scene* scene, bool isEnabled)
	{
		type = GUI::DEBUG_TOOLS;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};