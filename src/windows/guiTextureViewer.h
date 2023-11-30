#pragma once
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"
#include "interaction/selection.h"
#include "tools/state.h"

class GUITextureViewer : public IGUIWindow
{
private:
	State* mState = nullptr;
	Scene* mScene = nullptr;

	std::string mSelectedTexture = "";
	const float TEXTURE_SIZE = 256.0f;
	const std::vector<std::string> TEXTURE_TYPES = { "Diffuse", "Ambient", "Specular", "Normal", "Height", "Alpha" };
public:
	void Draw() override
	{
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Texture Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Draw selected texture
			if (mSelectedTexture != "")
			{
				Texture* curTexture = mScene->GetTexture(mSelectedTexture);
				int type = (int)curTexture->type;
				GUIWindowUtils::InputText("Name", curTexture->name);
				GUIWindowUtils::InputText("Path", curTexture->path);
				curTexture->type = (TextureType)GUIWindowUtils::Dropdown("Type", type, TEXTURE_TYPES);
				GUIWindowUtils::Image(curTexture->id, TEXTURE_SIZE);
				ImGui::Separator();
			}

			// Draw all textures
			const auto& textures = mScene->GetTextures()->Data();
			float size = ImGui::GetTextLineHeight();
			for (auto iter = textures.begin(); iter != textures.end(); ++iter)
			{
				// Skip the texture if it is for internal use only
				if (iter->second->IsInternal())
					continue;

				GUIWindowUtils::Image(iter->second->id, size);
				ImGui::SameLine();

				GUIWindowUtils::Deselectable(iter->first, mSelectedTexture, iter->first);
			}
		}
		ImGui::End();
	}

	GUITextureViewer(State* state, Scene* scene, bool isEnabled)
		: mState(state), mScene(scene)
	{
		mType = GUI::TEXTURE_VIEWER;
		mIsEnabled = isEnabled;
	}
};