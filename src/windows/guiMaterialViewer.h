#pragma once
#include "interfaces/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"
#include "interaction/selection.h"
#include "tools/state.h"

class GUIMaterialViewer : public IGUIWindow
{
private:
	State* mState = nullptr;
	Scene* mScene = nullptr;
public:
	void Draw() override
	{
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Material Viewer##Window", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			IEntity* sel = mState->GetSel()->GetSelectedEntity();
			MaterialContainer* materialList = mScene->GetMaterials();
			if (sel != nullptr && sel->GetMaterial() != nullptr)
			{
				const auto& mats = materialList->Data();
				Material* selMat = sel->GetMaterial();
				// Draw all materials
				float size = ImGui::GetTextLineHeight();
				float spacing = 5.0f;
				for (auto iter = mats.begin(); iter != mats.end(); ++iter)
				{
					// Skip the material if it is for internal use only
					if (iter->second->IsInternal())
						continue;

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
				{
					mScene->SetEntityMaterial(sel, selMat);
				}
			}
			else
			{
				ImGui::Text("No selection.");
			}
		}
		ImGui::End();
	}

	GUIMaterialViewer(State* state, Scene* scene, bool isEnabled)
		: mState(state), mScene(scene)
	{
		mType = GUI::MATERIAL_VIEWER;
		mIsEnabled = isEnabled;
	}
};