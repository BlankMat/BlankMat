#pragma once
#include "interfaces/iGUIWindow.h"
#include "guiWindowUtils.h"
#include "tools/state.h"
#include "rendering/scene.h"

class GUIMaterialEditor : public IGUIWindow
{
protected:
	State* mState = nullptr;
	Scene* mScene = nullptr;

	// Local state
	std::string mSelColor = "";
public:
	void Draw() override
	{
		if (ImGui::Begin("Material Editor", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			IEntity* selEntity = mState->GetSel()->GetSelectedEntity();
			Material* selMat = (selEntity != nullptr) ? selEntity->GetMaterial() : nullptr;
			Texture* selTexture = mScene->GetTextures()->GetSelectedItem();
			if (selMat != nullptr)
			{
				selMat->name = GUIWindowUtils::InputText("##matName", selMat->name);

				float size = ImGui::GetTextLineHeight();
				float spacing = 15.0f;

				ImVec2 pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->kd, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				GUIWindowUtils::Deselectable("Diffuse", mSelColor, std::string("kd"));
				if (mSelColor == "kd")
					selMat->kd = GUIWindowUtils::ColorPicker("##DiffuseColor", selMat->kd);

				pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->ka, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				GUIWindowUtils::Deselectable("Ambient", mSelColor, std::string("ka"));
				if (mSelColor == "ka")
					selMat->ka = GUIWindowUtils::ColorPicker("##AmbientColor", selMat->ka);

				pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->ks, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				GUIWindowUtils::Deselectable("Specular", mSelColor, std::string("ks"));
				if (mSelColor == "ks")
					selMat->ks = GUIWindowUtils::ColorPicker("##SpecularColor", selMat->ks);

				pos = ImGui::GetCursorScreenPos();
				GUIWindowUtils::DrawColor(selMat->ke, pos, size);
				ImGui::Dummy(ImVec2(size, size));
				ImGui::SameLine();
				GUIWindowUtils::Deselectable("Emissive", mSelColor, std::string("ke"));
				if (mSelColor == "ke")
					selMat->ke = GUIWindowUtils::ColorPicker("##EmissiveColor", selMat->ke);

				selMat->ns = GUIWindowUtils::InputFloat("Specular Exponent", selMat->ns);
				selMat->ni = GUIWindowUtils::InputFloat("Index of Refraction", selMat->ni);
				selMat->d = GUIWindowUtils::InputFloat("Alpha", selMat->d);
				selMat->illum = GUIWindowUtils::InputInt("Illumination Mode", selMat->illum);

				GUIWindowUtils::TextureSelect("Diffuse", selMat->map_kd, selTexture);
				GUIWindowUtils::TextureSelect("Ambient", selMat->map_ka, selTexture);
				GUIWindowUtils::TextureSelect("Specular", selMat->map_ks, selTexture);
				GUIWindowUtils::TextureSelect("Normal", selMat->map_bump, selTexture);
				GUIWindowUtils::TextureSelect("Height", selMat->map_ns, selTexture);
				GUIWindowUtils::TextureSelect("Alpha", selMat->map_d, selTexture);

				if (selTexture != mScene->GetTextures()->GetSelectedItem())
				{
					mScene->GetTextures()->Select(selTexture);
				}
			}
			else
			{
				ImGui::Text("No material selected");
			}
		}
		ImGui::End();
	}

	GUIMaterialEditor(State* state, Scene* scene, bool isEnabled)
		: mState(state), mScene(scene)
	{
		mType = GUI::MATERIAL_EDITOR;
		mIsEnabled = isEnabled;
	}
};