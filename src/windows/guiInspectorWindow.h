#pragma once
#include "iGUIWindow.h"
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
				std::string selName = sel->GetName();
				ImGui::InputText("Selected:", &selName);
				if (selName != sel->GetName())
					sel->SetName(selName);

				// Position
				glm::vec3 pos = sel->GetPos();
				float posInput[3] = { pos.x, pos.y, pos.z };
				ImGui::InputFloat3("Position", posInput);
				sel->SetPos(Vec3FromFloats(posInput));

				// Rotation
				glm::vec3 rot = sel->GetRot();
				float rotInput[3] = { rot.x, rot.y, rot.z };
				ImGui::InputFloat3("Rotation", rotInput);
				sel->SetRot(Vec3FromFloats(rotInput));

				// Scale
				glm::vec3 scale = sel->GetScale();
				float scaleInput[3] = { scale.x, scale.y, scale.z };
				ImGui::InputFloat3("Scale", scaleInput);
				sel->SetScale(Vec3FromFloats(scaleInput));

				// Shader
				std::unordered_map<std::string, Shader*>& shaders = mScene->GetShaderList();
				Shader* shader = sel->GetShader();
				if (shader != nullptr && ImGui::BeginListBox("Shader"))
				{
					for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
					{
						std::string itemName = iter->first;
						unsigned int itemID = iter->second->ID;
						bool isSelected = (itemID == shader->ID);
						if (ImGui::Selectable(itemName.c_str(), &isSelected))
							shader = iter->second;

						if (isSelected)
							ImGui::SetItemDefaultFocus();
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