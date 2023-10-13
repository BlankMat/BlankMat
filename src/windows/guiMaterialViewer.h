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

		if (ImGui::Begin("Material Viewer"))
		{
			if (ImGui::BeginMenu("Colors"))
			{
				float sz = ImGui::GetTextLineHeight();
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
					ImVec2 p = ImGui::GetCursorScreenPos();
					ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
					ImGui::Dummy(ImVec2(sz, sz));
					ImGui::SameLine();
					ImGui::MenuItem(name);
				}
				ImGui::EndMenu();
			}
			ImGui::End();
		}
	}

	GUIMaterialViewer(State* state, Scene* scene, bool isEnabled)
	{
		type = GUI::MATERIAL_BAR;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};