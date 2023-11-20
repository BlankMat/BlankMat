#pragma once
#include "glIncludes.h"
#include <string>
#include <iostream>

struct Texture
{
	unsigned int id;
	std::string name;
	std::string type;
	std::string path;

    // Loads the given texture from a file
    static unsigned int TextureFromFile(const std::string& directory, const std::string& name, int& out_width, int& out_height, bool gamma = false)
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
    }

	Texture::Texture(const unsigned int _id = -1, const std::string& _type = "", const std::string& _path = "", const std::string& _name = "")
		: id(_id), type(_type), path(_path), name(_name) {}

	Texture::Texture(const std::string& _type, const std::string& _directory, const std::string& _path, const std::string& _name)
		: type(_type), path(_path), name(_name)
	{
		int width, height;
		id = TextureFromFile(_directory, _path, width, height);
	}
};