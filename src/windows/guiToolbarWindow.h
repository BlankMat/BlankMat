#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "iGUIWindow.h"
#include "selection.h"
#include "stb_image.h"
#include <vector>
#include <iostream>
#include "string.h"

#define NUMBER_OF_TOOLS 5

class GUIToolbarWindow : public IGUIWindow
{
private: 
	std::string getToolName(Tool toolEnum)
	{
		switch (toolEnum)
		{
			case Tool::NONE:
				return "None";
				break;
			case Tool::SELECT:
				return "Select";
				break;
			case Tool::MOVE:
				return "Move";
				break;
			case Tool::ROTATE:
				return "Rotate";
				break;
			case Tool::SCALE:
				return "Scale";
				break;
			case Tool::EXTRUDE:
				return "Extrude";
				break;	
		}
		return "";
	}
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
	{
		stbi_set_flip_vertically_on_load(false);

		stbi__vertically_flip_on_load_global = false;
		// Load from file
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return false;

		// Create a OpenGL texture identifier
		GLuint image_texture;
		glGenTextures(1, &image_texture);
		glBindTexture(GL_TEXTURE_2D, image_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);

		*out_texture = image_texture;
		*out_width = image_width;
		*out_height = image_height;

		// TODO: Refactor using the state variable when available
		stbi_set_flip_vertically_on_load(true);

		return true;
	}

protected:
	Selection* mSelection;
	std::vector<GLuint> mTextureIDs;
	std::vector<ImVec2> mTextureDims;
public:
	void Draw() override
	{
		// Don't draw disabled GUI
		if(!mIsEnabled)
		{
			return;
		}
		// Name the window
		ImGui::Begin("Toolbar");

		// Add buttons and call functions if clicked
		for(int i = 0; i < NUMBER_OF_TOOLS; ++i)
		{
			bool isPressed = ImGui::ImageButton(getToolName(Tool(i+1)).c_str(),(void*)(intptr_t)mTextureIDs[i],mTextureDims[i]);
			if(isPressed)
			{
				mSelection->SetTool(Tool(i+1));
			}
		}

		ImGui::End();
	}

	GUIToolbarWindow(Selection* selection, bool isEnabled)
	{
		type = GUI::TOOLBAR;
		mSelection = selection;
		mIsEnabled = isEnabled;
		std::string filePath("..\\resources\\icons\\toolIcons\\");
		std::string fileExtension(".png");
		for(int i = 0; i < NUMBER_OF_TOOLS; ++i)
		{
			int my_image_width = 0;
			int my_image_height = 0;
			GLuint my_image_texture = 0;
			std::string toolName(getToolName(Tool(i+1)));
			std::string fullFilePath = filePath+ toolName+fileExtension;
			bool ret = LoadTextureFromFile(fullFilePath.c_str() , &my_image_texture, &my_image_width, &my_image_height);
			if(!ret)
			{
				std::cout << "Failed to load the "+toolName+" icon" << std::endl;
			}
			mTextureIDs.push_back(my_image_texture);
			mTextureDims.push_back(ImVec2(my_image_width,my_image_height));
		}
	}
};