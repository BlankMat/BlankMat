#pragma once
#include "iGUIWindow.h"
#include "guiWindowUtils.h"
#include "tools/state.h"
#include "rendering/scene.h"

class GUIInspectorWindow : public IGUIWindow
{
protected:
	State* mState;
	Scene* mScene;
public:
	void Draw() override
	{
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Inspector", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			IEntity* sel = mState->GetSel()->GetSelectedEntity();
			bool isSelected = (sel != nullptr);

			// Only show options if selected
			if (!isSelected)
			{
				ImGui::Text("Selected: None");
			}
			else
			{
				// Change name of selection
				sel->SetName(
					GUIWindowUtils::InputText("Selected", sel->GetName()));

				// Position
				sel->SetPos(
					GUIWindowUtils::InputVec3("Position", sel->GetPos()));

				// Rotation
				sel->SetRot(
					GUIWindowUtils::InputVec3("Rotation", sel->GetRot()));

				// Scale
				sel->SetScale(
					GUIWindowUtils::InputVec3("Scale", sel->GetScale()));

				// Enabled
				sel->Enable(
					GUIWindowUtils::Checkbox("Enabled", sel->IsEnabled()));

				// Shader
				std::unordered_map<std::string, Shader*>& shaders = mScene->GetShaderList();
				Shader* shader = sel->GetShader();
				if (shader != nullptr && ImGui::BeginListBox("Shader"))
				{
					for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
					{
						GUIWindowUtils::Selectable(iter->first, shader, iter->second);
					}
					ImGui::EndListBox();
					sel->SetShader(shader);
				}
			}
		}
		ImGui::End();
	}

	GUIInspectorWindow(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::INSPECTOR;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};