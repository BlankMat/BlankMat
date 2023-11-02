#pragma once
#include "renderingHelpers.h"
#include "tools/state.h"
#include "files/fileSystem.h"
#include "rendering/scene.h"
#include "windows/iGUIWindow.h"
#include "interaction/selection.h"
#include <vector>
#include <iostream>
#include <string>

constexpr auto MODES_DIR = "resources/icons/modeIcons";

class GUIToolModeWindow : public IGUIWindow
{
protected:
    const std::string mModeFileNames[3] = {"Mesh.png", "Vert.png", "Face.png"};
    State* mState;
    Scene* mScene;
    std::vector<unsigned int> mTextureIDs;
    ImVec2 mIconSize;
public:
    void Draw() override
    {
        if (ImGui::Begin("Tool Modes", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Selection* curSel = mState->GetSel();
            //Tool curTool = curSel->GetTool();
            SelMode curMode = curSel->GetSelMode();

            for (SelMode mode = SelMode::MESH; mode != SelMode::LAST; mode = (SelMode)((int)mode + 1))
            {
                int intM = (int)mode;
                ImVec4 newTint = ImVec4(1, 1, 1, 1);
                // If Tool t is the currently selected tool, tint the button to make it look darker.
                if (curMode == mode)
                {
                    newTint = ImVec4(0.5, 0.5, 0.5, 0.5);
                }
#pragma warning(suppress: 4312)
                bool isPressed = ImGui::ImageButton((void*)mTextureIDs[intM], mIconSize, ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), newTint);
                if (isPressed)
                {
                    curSel->SetSelMode(mode);
                    curMode = mode;
                }
            }
        }

        ImGui::End();
    }

    GUIToolModeWindow(State* state, Scene* scene, bool isEnabled)
    {
        mType = GUI::MODEBAR;
        mIsEnabled = isEnabled;
        mState = state;
        mScene = scene;
        mIconSize = ImVec2(50, 50);

        stbi_set_flip_vertically_on_load(false);
        for (SelMode mode = SelMode::MESH; mode != SelMode::LAST; mode = (SelMode)((int)mode+1))
        {
            std::string fileName = mModeFileNames[(int)mode];
            //std::string fullFilePath = TOOLSDIR + std::string("/")+ fileName;
            int widthDim = 0;
            int heightDim = 0;
            unsigned int textureID = TextureFromFile(FileSystem::GetPath(MODES_DIR), fileName, widthDim, heightDim, false);
            mTextureIDs.push_back(textureID);
        }
        stbi_set_flip_vertically_on_load(state->flipTextures);
    }
};  