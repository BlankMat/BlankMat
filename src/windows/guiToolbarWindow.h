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

#define TOOLSDIR "resources/icons/toolIcons"
#define SelectFileName "Select.png"
#define MoveFileName "Move.png"
#define RotateFileName "Rotate.png"
#define ScaleFileName "Scale.png"
#define ExtrudeFileName "Extrude.png"

class GUIToolbarWindow : public IGUIWindow
{
protected:
    State* mState;
    Scene* mScene;
    const std::string fileNames[5] = {SelectFileName,MoveFileName,RotateFileName,ScaleFileName,ExtrudeFileName};
    std::vector<ImTextureID>  mTextureIDs;
    std::vector<ImVec2> mDims;
public:
    void Draw() override
    {
        ImGui::Begin("Toolbar");
		Selection* curSel = mState->GetSel();
        Tool curTool = curSel->GetTool();
        for(Tool t = Tool::SELECT; t != Tool::LAST; t = (Tool)((int)t+1))
        {
            int offsetT = (int)t-1;
            ImVec4 newTint = ImVec4(1,1,1,1);
            // If Tool t is the currently selected tool, tint the button to make it look darker.
            if(curTool == t)
            {
                newTint = ImVec4(0.5,0.5,0.5,0.5);
            }
            bool isPressed = ImGui::ImageButton(mTextureIDs[offsetT], mDims[offsetT],ImVec2(0,0),ImVec2(1,1),-1,ImVec4(0,0,0,0),newTint);
            if(isPressed)
            {
                curSel->SetTool(t);
                curTool = t;
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
        stbi_set_flip_vertically_on_load(false);
        for(Tool t = Tool::SELECT; t != Tool::LAST; t = (Tool)((int)t+1))
        {
            std::string fileName = fileNames[(int)t-1];
            //std::string fullFilePath = TOOLSDIR + std::string("/")+ fileName;
            int widthDim = 0;
            int heightDim = 0;
            ImTextureID textureID = (ImTextureID)TextureFromFile(FileSystem::GetPath(TOOLSDIR),fileName,widthDim,heightDim, false);
            mTextureIDs.push_back(textureID);
            mDims.push_back(ImVec2(widthDim,heightDim));
        }
        stbi_set_flip_vertically_on_load(true);
    }
};