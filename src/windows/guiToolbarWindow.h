#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include "iGUIWindow.h"
#include "selection.h"
#include "stb_image.h"
#include <vector>
#include <iostream>
#include "string.h"
#include "renderingHelpers.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include "files/fileSystem.h"

constexpr char* TOOLS_DIR = "resources/icons/toolIcons";
constexpr char* SELECT_ICON = "Select.png";
constexpr char* MOVE_ICON = "Move.png";
constexpr char* ROTATE_ICON = "Rotate.png";
constexpr char* SCALE_ICON = "Scale.png";
constexpr char* EXTRUDE_ICON = "Extrude.png";

class GUIToolbarWindow : public IGUIWindow
{
protected:
    State* mState;
    Scene* mScene;
    const std::string fileNames[5] = {SELECT_ICON,MOVE_ICON,ROTATE_ICON,SCALE_ICON,EXTRUDE_ICON};
    std::vector<unsigned int> mTextureIDs;
    ImVec2 mIconSize;
public:
    void Draw() override
    {
        if (ImGui::Begin("Toolbar", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Selection* curSel = mState->GetSel();
            Tool curTool = curSel->GetTool();
            for (Tool tool = Tool::SELECT; tool != Tool::LAST; tool = (Tool)((int)tool + 1))
            {
                int offsetT = (int)tool - 1;
                ImVec4 newTint = ImVec4(1, 1, 1, 1);
                // If Tool t is the currently selected tool, tint the button to make it look darker.
                if (curTool == tool)
                {
                    newTint = ImVec4(0.5, 0.5, 0.5, 0.5);
                }
#pragma warning(suppress: 4312)
                bool isPressed = ImGui::ImageButton((void*)mTextureIDs[offsetT], mIconSize, ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), newTint);
                if (isPressed)
                {
                    curSel->SetTool(tool);
                    curTool = tool;
                }
            }
        }

        ImGui::End();
    }

    GUIToolbarWindow(State* state, Scene* scene, bool isEnabled)
    {
        mType = GUI::TOOLBAR;
        mIsEnabled = isEnabled;
        mState = state;
        mScene = scene;
        mIconSize = ImVec2(50, 50);

        stbi_set_flip_vertically_on_load(false);
        for(Tool tool = Tool::SELECT; tool != Tool::LAST; tool = (Tool)((int)tool+1))
        {
            std::string fileName = fileNames[(int)tool-1];
            //std::string fullFilePath = TOOLSDIR + std::string("/")+ fileName;
            int widthDim = 0;
            int heightDim = 0;
            unsigned int textureID = TextureFromFile(FileSystem::GetPath(TOOLS_DIR), fileName, widthDim, heightDim, false);
            mTextureIDs.push_back(textureID);
        }
        stbi_set_flip_vertically_on_load(state->flipTextures);
    }
};