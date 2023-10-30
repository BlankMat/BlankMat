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

				ImGui::Separator();
				ImGui::Text("Internal Matrices (readonly)");
				GUIWindowUtils::InputMat4("Model", sel->GetModelMatrix());
				ImGui::Separator();
				GUIWindowUtils::InputMat4("Self Model", sel->GetSelfModelMatrix());
				ImGui::Separator();
				GUIWindowUtils::InputMat4("Parent Model", sel->GetParentModelMatrix());
				ImGui::Separator();
				GUIWindowUtils::InputMat3("Normal Model", sel->GetNormalModelMatrix());
				ImGui::Separator();
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