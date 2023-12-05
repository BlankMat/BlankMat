#pragma once
#include "interfaces/iGUIContainerWindow.h"
#include "windows/guiWindowUtils.h"

class GUILightViewer : public IGUIContainerWindow<Light>
{
protected:
    IContainer<Light>* GetContainer() override
    {
        return mScene->GetLights();
    }

    void DisplaySelectedItem() override
    {
        Light* selection = mScene->GetLights()->GetSelectedItem();
        if (selection != nullptr)
            return;

        ImGui::Text("No light selected");
    }

    void SelectItem(Light* selection) override
    {
        mState->GetSel()->SelectElement(selection);
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