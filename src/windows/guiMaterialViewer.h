#pragma once
#include "iGUIWindow.h"
#include "guiWindowUtils.h"
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
				float size = ImGui::GetTextLineHeight();
				float spacing = 5.0f;
				for (auto iter = mats.begin(); iter != mats.end(); ++iter)
				{
					ImVec2 pos = ImGui::GetCursorScreenPos();
					GUIWindowUtils::DrawColor(iter->second->kd, pos, size);
					pos.x += size + spacing;
					GUIWindowUtils::DrawColor(iter->second->ka, pos, size);
					pos.x += size + spacing;
					GUIWindowUtils::DrawColor(iter->second->ks, pos, size);
					ImGui::Dummy(ImVec2((size + spacing)*3, size));
					ImGui::SameLine();

					GUIWindowUtils::Selectable(iter->first, selMat, iter->second);
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
		mType = GUI::MATERIAL_VIEWER;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};