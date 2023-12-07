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
        if (ImGui::Begin("Material Picker", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (mState->isEditingMaterial)
            {
                // List all textures for choosing
                const auto& materials = mScene->GetMaterials()->Data();
                for (auto iter = materials.begin(); iter != materials.end(); ++iter)
                {
                    GUIWindowUtils::MaterialSelect(iter->first, iter->second, mCurSelectedMaterial, 5.0f);
                }

                // If any material was chosen, update the requested material
                if (mState->isEditingMaterial && mCurSelectedMaterial != nullptr)
                {
                    mState->materialInEdit = mCurSelectedMaterial;
                }
            }
            else
            {
                ImGui::Text("Select material to edit");
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