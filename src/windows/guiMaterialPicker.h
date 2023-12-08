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
        if (!mIsEnabled || !mState->GetSel()->IsSelectingMaterial())
            return;

        if (ImGui::Begin("Material Picker", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            // List all textures for choosing
            const auto& materials = mScene->GetMaterials()->Data();
            for (auto iter = materials.begin(); iter != materials.end(); ++iter)
            {
                if (iter->second == nullptr || iter->second->IsInternal())
                    continue;
                GUIWindowUtils::MaterialSelect(iter->first, iter->second, mCurSelectedMaterial, 5.0f);
            }

            // If any material was chosen, update the requested material
            if (mCurSelectedMaterial != nullptr)
            {
                mState->GetSel()->SetMaterialInSelect(mCurSelectedMaterial);
                mCurSelectedMaterial = nullptr;
            }
        }
        ImGui::End();
    }

    GUIMaterialPicker(State* state, Scene* scene, bool isEnabled)
    {
        mState = state;
        mScene = scene;
        mType = GUI::MATERIAL_PICKER;
        mIsEnabled = isEnabled;
    }
};