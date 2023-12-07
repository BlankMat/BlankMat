#pragma once
#include "interfaces/iGUIWindow.h"
#include "interaction/state.h"
#include "rendering/scene.h"

class GUIMaterialPicker : public IGUIWindow
{
protected:
    State* mState = nullptr;
    Scene* mScene = nullptr;
    Material* mCurSelectedMaterial = nullptr;
    
public:
    void Draw() override
    {
        bool enabled = mIsEnabled && mState->materialInEdit != nullptr;
        if (ImGui::Begin("Material Picker", &enabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            // List all textures for choosing
            const auto& materials = mScene->GetMaterials()->Data();
            for (auto iter = materials.begin(); iter != materials.end(); ++iter)
            {
                GUIWindowUtils::MaterialSelect(iter->first, iter->second, mCurSelectedMaterial, 5.0f);
            }

            // If any material was chosen, update the requested material
            if (mState->materialInEdit != nullptr && mCurSelectedMaterial != nullptr)
            {
                *mState->materialInEdit = mCurSelectedMaterial;
                mCurSelectedMaterial = nullptr;
                mState->materialInEdit = nullptr;
            }
        }
    }

    GUIMaterialPicker(State* state, Scene* scene, bool isEnabled)
    {
        mState = state;
        mScene = scene;
        mType = GUI::MATERIAL_PICKER;
        mIsEnabled = isEnabled;
    }
};