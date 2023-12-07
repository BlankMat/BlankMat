#pragma once
#include "interfaces/iGUIWindow.h"
#include "interaction/state.h"
#include "rendering/scene.h"

class GUITexturePicker : public IGUIWindow
{
protected:
    State* mState = nullptr;
    Scene* mScene = nullptr;
    Texture* mCurSelectedTexture = nullptr;

public:
    void Draw() override
    {
        if (ImGui::Begin("Texture Picker", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (mState->isEditingTexture)
            {
                // List all textures for choosing
                const auto& textures = mScene->GetTextures()->Data();
                for (auto iter = textures.begin(); iter != textures.end(); ++iter)
                {
                    GUIWindowUtils::TextureSelect(iter->first, iter->second, mCurSelectedTexture);
                }

                // If any texture was chosen, update the requested texture
                if (mState->isEditingTexture && mCurSelectedTexture != nullptr)
                {
                    mState->textureInEdit = mCurSelectedTexture;
                }
            }
            else
            {
                ImGui::Text("Select texture to edit");
            }
        }
        ImGui::End();
    }

    GUITexturePicker(State* state, Scene* scene, bool isEnabled)
    {
        mState = state;
        mScene = scene;
        mType = GUI::TEXTURE_PICKER;
        mIsEnabled = isEnabled;
    }
};