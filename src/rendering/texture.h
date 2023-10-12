#pragma once
#include <string>
#include <iostream>

#define TEXTURE_DIR "resources/textures"

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;

	Texture::Texture(unsigned int _id = -1, std::string _type = "", std::string _path = "")
		: id(_id), type(_type), path(_path) {}

	Texture::Texture(std::string _type, std::string _directory, std::string _path)
	{
		type = _type;
		path = _path;
		id = TextureFromFile(_directory, _path);
	}

	// Loads the given texture from a file
	static unsigned int TextureFromFile(const std::string& directory, const std::string& name, bool gamma = false)
	{
		std::string fileName = directory + '/' + name;
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
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
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
};