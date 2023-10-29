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

#define MODESDIR "resources/icons/modeIcons"



class GUIToolModeWindow : public IGUIWindow
{
protected:
    const std::string mModeFileNames[3] = {"Mesh.png", "Vert.png", "Face.png"};
    State* mState;
    Scene* mScene;
    //const std::string fileNames[5] = {SelectModesFileName,MoveModesFileName,RotateModesFileName,ScaleModesFileName,ExtrudeModesFileName};
    std::vector<ImTextureID>  mTextureIDs;
    std::vector<ImVec2> mDims;
public:
    void Draw() override
    {
        ImGui::Begin("Tool Modes");
		Selection* curSel = mState->GetSel();
        //Tool curTool = curSel->GetTool();
        SelMode curMode = curSel->GetSelMode();
        
        for(SelMode m = SelMode::MESH; m != SelMode::LAST; m = (SelMode)((int)m+1))
        {
            int intM = (int)m;
            ImVec4 newTint = ImVec4(1,1,1,1);
            // If Tool t is the currently selected tool, tint the button to make it look darker.
            if(curMode == m)
            {
                newTint = ImVec4(0.5,0.5,0.5,0.5);
            }
            bool isPressed = ImGui::ImageButton(mTextureIDs[intM], mDims[intM],ImVec2(0,0),ImVec2(1,1),-1,ImVec4(0,0,0,0),newTint);
            if(isPressed)
            {
                curSel->SetSelMode(m);
                curMode = m;
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
        stbi_set_flip_vertically_on_load(false);
        for(SelMode m = SelMode::MESH; m != SelMode::LAST; m = (SelMode)((int)m+1))
        {
            std::string fileName = mModeFileNames[(int)m];
            //std::string fullFilePath = TOOLSDIR + std::string("/")+ fileName;
            int widthDim = 0;
            int heightDim = 0;
            ImTextureID textureID = (ImTextureID)TextureFromFile(FileSystem::GetPath(MODESDIR),fileName,widthDim,heightDim, false);
            mTextureIDs.push_back(textureID);
            mDims.push_back(ImVec2(widthDim,heightDim));
        }
        stbi_set_flip_vertically_on_load(true);
    }
};  