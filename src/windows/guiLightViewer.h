#pragma once
#include "interfaces/iGUIContainerWindow.h"
#include "windows/guiWindowUtils.h"

class GUILightViewer : public IGUIContainerWindow<Light>
{
protected:
    Light* GetSelectedItem() override
    {
        return mScene->GetLight();
    }

    IContainer<Light>* GetContainer() override
    {
        return mScene->GetLights();
    }

    void DisplaySelectedItem(Light* selection) override
    {
        if (selection == nullptr)
            return;

        // Select light type
        LightType type = selection->GetType();
        if (ImGui::BeginListBox("Type"))
        {
            GUIWindowUtils::Selectable("Point", type, LightType::POINT);
            GUIWindowUtils::Selectable("Directional", type, LightType::DIR);
            GUIWindowUtils::Selectable("Spot", type, LightType::SPOT);
            selection->SetType(type);
            ImGui::EndListBox();
        }

        // Light position
        if (type == LightType::SPOT || type == LightType::POINT)
        {
            selection->SetOffset(
                GUIWindowUtils::InputVec3("Position", selection->GetOffset()));
        }

        // Light direction
        if (type == LightType::SPOT || type == LightType::DIR)
        {
            selection->SetDir(
                GUIWindowUtils::InputVec3("Direction", selection->GetDir()));
        }

        // Light Diffuse, Ambient, and Specular
        selection->SetKD(
            GUIWindowUtils::InputFloat("Diffuse", selection->GetKD()));
        selection->SetKA(
            GUIWindowUtils::InputFloat("Ambient", selection->GetKA()));
        selection->SetKS(
            GUIWindowUtils::InputFloat("Specular", selection->GetKS()));

        // Light gamma
        selection->SetGamma(
            GUIWindowUtils::Checkbox("Gamma", selection->GetGamma()));

        // Show attributes of point and spotlights
        if (type == LightType::POINT || type == LightType::SPOT)
        {
            // Range coefficient
            selection->SetRange(
                GUIWindowUtils::InputFloat("Range", selection->GetRange()));
        }
        // Show attributes of spotlight
        if (type == LightType::SPOT)
        {
            // Inner radius
            selection->SetSpotInnerRadius(
                GUIWindowUtils::InputFloat("Inner Radius", selection->GetSpotInnerRadius()));
            // Outer radius
            selection->SetSpotOuterRadius(
                GUIWindowUtils::InputFloat("Outer Radius", selection->GetSpotOuterRadius()));
        }

        // Light color
        selection->SetBaseColor(
            GUIWindowUtils::ColorPicker("Light Color", selection->GetBaseColor()));
    }

public:
    GUILightViewer(State* state, Scene* scene, bool isEnabled)
    {
        mState = state;
        mScene = scene;
        mType = GUI::LIGHT_VIEWER;
        mIsEnabled = isEnabled;
        mMustSelect = true;
        mWindowName = "Light Viewer";
    }
};