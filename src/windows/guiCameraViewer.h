#pragma once
#include "interfaces/iGUIContainerWindow.h"
#include "windows/guiWindowUtils.h"

/// <summary>
/// UI window to select a camera and to display camera properties
/// </summary>
class GUICameraViewer : public IGUIContainerWindow<Camera>
{
protected:
    void AddNewItem(const std::string& name) override
    {
        mScene->AddCamera(name, new Camera(mState->GetActionStack(), name, ""));
    }

    IContainer<Camera>* GetContainer() override
    {
        return mScene->GetCameras();
    }

    void DisplaySelectedItem() override
    {
        Camera* selection = mScene->GetCameras()->GetSelectedItem();
        if (selection != nullptr)
            return;

        ImGui::Text("No camera selected");
    }

    void SelectItem(Camera* selection) override
    {
        mState->GetSel()->SelectElement(selection);
    }


public:
    GUICameraViewer(State* state, Scene* scene, bool isEnabled)
    {
        mState = state;
        mScene = scene;
        mType = GUI::CAMERA_VIEWER;
        mIsEnabled = isEnabled;
        mMustSelect = true;
        mWindowName = "Camera Viewer";
    }
};