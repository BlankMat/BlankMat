#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "files/fileSystem.h"
#include "rendering/scene.h"
#include "interfaces/iGUIWindow.h"
#include "interaction/selection.h"
#include <vector>
#include <iostream>
#include <string>

constexpr auto MODES_DIR = "resources/icons/modeIcons";

class GUIToolModeWindow : public IGUIWindow
{
protected:
    State* mState = nullptr;
    Scene* mScene = nullptr;
    ImVec2 mIconSize = ImVec2(50, 50);

    const std::string mModeFileNames[3] = { "Mesh.png", "Vert.png", "Face.png" };
    std::vector<unsigned int> mTextureIDs;
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
        : mState(state), mScene(scene)
    {
        mType = GUI::MODEBAR;
        mIsEnabled = isEnabled;

        stbi_set_flip_vertically_on_load(false);
        for (SelMode mode = SelMode::MESH; mode != SelMode::LAST; mode = (SelMode)((int)mode+1))
        {
            std::string fileName = mModeFileNames[(int)mode];
            //std::string fullFilePath = TOOLSDIR + std::string("/")+ fileName;
            int widthDim = 0;
            int heightDim = 0;
            unsigned int textureID = Texture::TextureFromFile(FileSystem::GetPath(MODES_DIR), fileName, widthDim, heightDim, false);
            mTextureIDs.push_back(textureID);
        }
        stbi_set_flip_vertically_on_load(state->flipTextures);
    }
};  