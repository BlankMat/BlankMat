#pragma once
#include "interfaces/iGUIContainerWindow.h"
#include "windows/guiWindowUtils.h"

/// <summary>
/// UI window to select a camera and to display camera properties
/// </summary>
class GUICameraViewer : public IGUIContainerWindow<Camera>
{
protected:
    IContainer<Camera>* GetContainer() override
    {
        return mScene->GetCameras();
    }

    void DisplaySelectedItem() override
    {
        Camera* selection = mScene->GetCameras()->GetSelectedItem();
        if (selection == nullptr)
            return;

        selection->GetTarget().Display();
        selection->GetPivot().Display();
        selection->GetOrthSize().Display();
        selection->GetFOV().Display();
        selection->GetNearClip().Display();
        selection->GetFarClip().Display();
        selection->IsPerspective().Display();
        selection->IsWireframe().Display();
        selection->IsRotatingAroundPivot().Display();
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