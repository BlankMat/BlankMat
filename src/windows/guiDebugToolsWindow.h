#pragma once
#include "iGUIWindow.h"
#include "selection.h"
#include "options.h"
#include "mathLib.h"
#include "rendering/modelScene.h"
#include "rendering/camera.h"

class GUIDebugToolsWindow : public IGUIWindow
{
protected:
	Options* mOptions;
	Selection* mSelection;
	ModelScene* mScene;
public:
	void Draw() override
	{
		// Don't draw disabled GUI
		if (!mIsEnabled)
			return;

		ImGui::Begin("Debug Tools");

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

		// Light settings
		ImGui::Text("Light Settings");
		ILight* light = mScene->GetLight();

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

		// Model settings
		Model* model = mScene->GetModel();
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

		// Debug settings
		ImGui::Text("Debug settings");
		ImGui::Checkbox("Disco Light", &mOptions->isDiscoLight);
		ImGui::Checkbox("Rotating Light", &mOptions->isRotatingLight);
		ImGui::Checkbox("Perspective", &mOptions->isPerspective);
		ImGui::Checkbox("Wireframe", &mOptions->wireframe);
		ImGui::End();

		// Handle resulting changes
		mScene->GetCamera()->SetPerspective(mOptions->isPerspective);
		OpenGLEnableWireframe(mOptions->wireframe);
	}

	GUIDebugToolsWindow(Selection* selection, ModelScene* scene, Options* options, bool isEnabled)
	{
		type = GUI::DEBUG_TOOLS;
		mOptions = options;
		mSelection = selection;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};