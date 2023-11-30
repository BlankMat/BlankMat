#pragma once
#include "iGUIWindow.h"
#include "guiWindowUtils.h"
#include "tools/state.h"
#include "rendering/scene.h"

/// <summary>
/// UI window to select a camera and to display camera properties
/// </summary>
class GUICameraViewer : public IGUIWindow
{
protected:
    State* mState = nullptr;
    Scene* mScene = nullptr;
public:
    void Draw() override
    {
        if (ImGui::Begin("Camera Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Camera* curCamera = mScene->GetCamera();
            if (curCamera != nullptr)
            {
                curCamera->GetPos().Display();
                curCamera->GetTarget().Display();
                curCamera->GetPivot().Display();
                curCamera->GetOrthSize().Display();
                curCamera->GetFOV().Display();
                curCamera->GetNearClip().Display();
                curCamera->GetFarClip().Display();
                curCamera->IsPerspective().Display();
                curCamera->IsWireframe().Display();
                curCamera->IsRotatingAroundPivot().Display();
            }

            // Display all cameras
            ImGui::Separator();
            std::string selCamera = mScene->GetCameras()->GetSelectedName();
            const auto& cameras = mScene->GetCameras()->Data();
            for (auto iter = cameras.begin(); iter != cameras.end(); ++iter)
            {
                GUIWindowUtils::Selectable(iter->first, selCamera, iter->first);
            }
            mScene->SetCamera(selCamera);
        }
        ImGui::End();
    }

    GUICameraViewer(State* state, Scene* scene, bool isEnabled)
        : mState(state), mScene(scene)
    {
        mType = GUI::CAMERA_VIEWER;
        mScene = scene;
    }
};