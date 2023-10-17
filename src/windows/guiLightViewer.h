#pragma once
#include "iGUIWindow.h"
#include "tools/state.h"
#include "rendering/scene.h"

class GUILightViewer : public IGUIWindow
{
protected:
    State* mState;
    Scene* mScene;
public:
    void Draw() override
    {
        ImGui::Begin("Light Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize);
        Light* light = mScene->GetLight();
        LightType type = light->GetType();

        // Select light type
        if (ImGui::BeginListBox("Type"))
        {
            // Point light
            bool isPointSelected = (type == LightType::POINT);
            if (ImGui::Selectable("Point", &isPointSelected))
                type = LightType::POINT;
            if (isPointSelected)
                ImGui::SetItemDefaultFocus();

            // Directional light
            bool isDirSelected = (type == LightType::DIR);
            if (ImGui::Selectable("Directional", &isDirSelected))
                type = LightType::DIR;
            if (isDirSelected)
                ImGui::SetItemDefaultFocus();

            // Spotlight
            bool isSpotSelected = (type == LightType::SPOT);
            if (ImGui::Selectable("Spot", &isSpotSelected))
                type = LightType::SPOT;
            if (isSpotSelected)
                ImGui::SetItemDefaultFocus();
            light->SetType(type);
            ImGui::EndListBox();
        }

        // Light position
        if (type == LightType::SPOT || type == LightType::POINT)
        {
            glm::vec3 lightPos = light->GetOffset();
            float lightPosInput[3] = { lightPos.x, lightPos.y, lightPos.z };
            ImGui::InputFloat3("Position", lightPosInput);
            light->SetOffset(Vec3FromFloats(lightPosInput));
        }

        // Light direction
        if (type == LightType::SPOT || type == LightType::DIR)
        {
            glm::vec3 lightDir = light->GetDir();
            float lightDirInput[3] = { lightDir.x, lightDir.y, lightDir.z };
            ImGui::InputFloat3("Direction", lightDirInput);
            light->SetDir(Vec3FromFloats(lightDirInput));
        }

        // Light KD
        float lightKD = light->GetKD();
        ImGui::InputFloat("Diffuse", &lightKD);
        light->SetKD(lightKD);

        // Light KA
        float lightKA = light->GetKA();
        ImGui::InputFloat("Ambient", &lightKA);
        light->SetKA(lightKA);

        // Light KS
        float lightKS = light->GetKS();
        ImGui::InputFloat("Specular", &lightKS);
        light->SetKS(lightKS);

        // Light gamma
        bool lightGamma = light->GetGamma();
        ImGui::Checkbox("Gamma", &lightGamma);
        light->SetGamma(lightGamma);

        // Show attributes of point and spotlights
        if (type == LightType::POINT || type == LightType::SPOT)
        {
            // Range coefficient
            float pointRange = light->GetPointRange();
            ImGui::InputFloat("Range", &pointRange);
            light->SetPointRange(pointRange);
        }
        // Show attributes of spotlight
        if (type == LightType::SPOT)
        {
            // Inner radius
            float spotInner = light->GetSpotInnerRadius();
            ImGui::InputFloat("Inner Radius", &spotInner);
            light->SetSpotInnerRadius(spotInner);

            // Outer radius
            float spotOuter = light->GetSpotOuterRadius();
            ImGui::InputFloat("Outer Radius", &spotOuter);
            light->SetSpotOuterRadius(spotOuter);
        }

        // Light color
        glm::vec3 lightColor = light->GetBaseColor();
        float lightColorInput[3] = { lightColor.x, lightColor.y, lightColor.z };
        ImGui::ColorPicker3("Light Color", lightColorInput);
        light->SetBaseColor(Vec3FromFloats(lightColorInput));

        ImGui::End();
    }

    GUILightViewer(State* state, Scene* scene, bool isEnabled)
    {
        mType = GUI::LIGHT_VIEWER;
        mIsEnabled = isEnabled;
        mState = state;
        mScene = scene;
    }
};