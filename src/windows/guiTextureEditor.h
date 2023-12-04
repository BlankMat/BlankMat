#pragma once
#include "interfaces/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"
#include "interaction/selection.h"
#include "tools/state.h"

class GUITextureEditor : public IGUIWindow
{
private:
	State* mState = nullptr;
	Scene* mScene = nullptr;
public:
	void Draw() override
	{
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Texture Editor", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			Texture* selTexture = mScene->GetTextures()->GetSelectedItem();
			// Draw selected texture
			if (selTexture != nullptr && !selTexture->IsInternal())
			{
				GUIWindowUtils::TextureEdit(selTexture);
			}
			else
			{
				ImGui::Text("No texture selected");
			}
		}
		ImGui::End();
	}

	GUITextureEditor(State* state, Scene* scene, bool isEnabled)
		: mState(state), mScene(scene)
	{
		mType = GUI::TEXTURE_EDITOR;
		mIsEnabled = isEnabled;
	}
};