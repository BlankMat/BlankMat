#pragma once
#include "iGUIWindow.h"
#include "guiWindowUtils.h"
#include "tools/state.h"
#include "rendering/scene.h"

class GUIMaterialEditor : public IGUIWindow
{
protected:
	State* mState;
	Scene* mScene;
	std::string mSelColor = "";
public:
	void Draw() override
	{
		if (ImGui::Begin("Material Editor", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			IEntity* selEntity = mState->GetSel()->GetSelectedEntity();
			Material* selMat = (selEntity != nullptr) ? selEntity->GetMaterial() : nullptr;
			if (selMat != nullptr)
			{
				selMat->name = GUIWindowUtils::InputText("##matName", selMat->name);

				float size = ImGui::GetTextLineHeight();
				float spacing = 15.0f;

				ImVec2 pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->kd, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				bool isSelected = GUIWindowUtils::Selectable("Diffuse", mSelColor, std::string("kd"));
				if (isSelected)
					selMat->kd = GUIWindowUtils::ColorPicker("Diffuse", selMat->kd);

				pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->ka, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				isSelected = GUIWindowUtils::Selectable("Ambient", mSelColor, std::string("ka"));
				if (isSelected)
					selMat->ka = GUIWindowUtils::ColorPicker("Ambient", selMat->ka);

				pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->ks, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				isSelected = GUIWindowUtils::Selectable("Specular", mSelColor, std::string("ks"));
				if (isSelected)
					selMat->ks = GUIWindowUtils::ColorPicker("Specular", selMat->ks);

				pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->ke, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				isSelected = GUIWindowUtils::Selectable("Emissive", mSelColor, std::string("ke"));
				if (isSelected)
					selMat->ke = GUIWindowUtils::ColorPicker("Emissive", selMat->ke);

				selMat->ns = GUIWindowUtils::InputFloat("Specular Exponent", selMat->ns);
				selMat->ni = GUIWindowUtils::InputFloat("Index of Refraction", selMat->ni);
				selMat->d = GUIWindowUtils::InputFloat("Alpha", selMat->d);
				selMat->illum = GUIWindowUtils::InputInt("Illumination Mode", selMat->illum);
			}
			else
			{
				ImGui::Text("No material selected");
			}
		}
		ImGui::End();
	}

	GUIMaterialEditor(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::MATERIAL_EDITOR;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};