#pragma once
#include "iGUIWindow.h"
#include "rendering/scene.h"
#include "tools/state.h"

class GUIMaterialViewer : public IGUIWindow
{
private:
	State* mState;
	Scene* mScene;
public:
	void Draw() override
	{
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Material Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			IEntity* sel = mState->GetSel()->GetSelectedEntity();
			std::unordered_map<std::string, Material*>& mats = mScene->GetMaterialList();
			if (sel != nullptr && sel->GetMaterial() != nullptr)
			{
				Material* selMat = sel->GetMaterial();
				// Draw all materials
				float sz = ImGui::GetTextLineHeight();
				float spacing = 5.0f;
				for (auto iter = mats.begin(); iter != mats.end(); ++iter)
				{
					glm::vec3 kd = iter->second->kd;
					glm::vec3 ka = iter->second->ka;
					glm::vec3 ks = iter->second->ks;
					ImU32 colorKD = ImGui::GetColorU32(ImVec4(kd.r, kd.g, kd.b, 1.0f));
					ImU32 colorKA = ImGui::GetColorU32(ImVec4(ka.r, ka.g, ka.b, 1.0f));
					ImU32 colorKS = ImGui::GetColorU32(ImVec4(ks.r, ks.g, ks.b, 1.0f));
					ImVec2 p = ImGui::GetCursorScreenPos();
					ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), colorKD);
					p.x += sz + spacing;
					ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), colorKA);
					p.x += sz + spacing;
					ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), colorKS);
					ImGui::Dummy(ImVec2((sz + spacing)*3, sz));
					ImGui::SameLine();

					bool isSelected = (selMat == iter->second);
					if (ImGui::Selectable(iter->first.c_str(), &isSelected))
						selMat = iter->second;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				// If the selected material changed, update sel
				if (selMat != sel->GetMaterial())
					sel->SetMaterial(selMat);
			}
			else
			{
				ImGui::Text("No selection.");
			}
		}
		ImGui::End();
	}

	GUIMaterialViewer(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::MATERIAL_BAR;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};