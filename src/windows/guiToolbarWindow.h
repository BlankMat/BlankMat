#pragma once
#define STB_IMAGE_IMPLEMENTATION
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


//a[5] = [SelectFileName,MoveFileName,RotateFileName,ScaleFileName,ExtrudeFileName];


class GUIToolbarWindow : public IGUIWindow
{
protected:
    State* mState;
    Scene* mScene;
    const std::string fileNames[5] = {SelectFileName,MoveFileName,RotateFileName,ScaleFileName,ExtrudeFileName};
    std::vector<ImTextureID>  mTextureIDs;
    std::vector<ImVec2> mDims;
    /*unsigned int TextureFromFile(const std::string& directory, const std::string& name, int& out_width, int& out_height, bool gamma = false)
	{
		std::string fileName = directory + '/' + name;
		unsigned int textureID;
		glGenTextures(1, &textureID);
		int nrComponents;
		unsigned char* data = stbi_load(fileName.c_str(), &out_width, &out_height, &nrComponents, 0);
		if (data)
		{
			GLenum format = GL_RGB;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, out_width, out_height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << name << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}*/
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