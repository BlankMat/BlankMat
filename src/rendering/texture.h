#pragma once
#include "glIncludes.h"
#include <string>
#include <iostream>

/// <summary>
/// An enumerator that lists all the allowed texture types
/// </summary>
enum class TextureType
{
    DIFFUSE = 0, 
    AMBIENT,
    SPECULAR,
    NORMAL,
    HEIGHT,
    ALPHA
};

/// <summary>
/// A structure that stores a texture file that has been read into the OpenGL context.
/// </summary>
struct Texture
{
private:
    /// <summary>
    /// Whether the texture is for internal use only
    /// </summary>
    bool mIsInternal = false;

public:
	/// <summary>
	/// OpenGL texture index
	/// </summary>
	unsigned int id;

	/// <summary>
	/// The name of the texture. Used to distinguish textures, so it should be unique.
	/// </summary>
	std::string name;

	/// <summary>
	/// The path of the file for the texture
	/// </summary>
	std::string path;

	/// <summary>
	/// The type of the texture
	/// </summary>
	TextureType type;

    /// <summary>
    /// Casts the given texture type into its shader equivalent string
    /// </summary>
    /// <param name="type">Texture Type</param>
    /// <returns>Shader texture type</returns>
    static std::string TextureToTypeString(TextureType type)
    {
        switch (type)
        {
        case TextureType::DIFFUSE:
            return "texture_diffuse";
        case TextureType::AMBIENT:
            return "texture_ambient";
        case TextureType::SPECULAR:
            return "texture_specular";
        case TextureType::NORMAL:
            return "texture_normal";
        case TextureType::HEIGHT:
            return "texture_height";
        case TextureType::ALPHA:
            return "texture_alpha";
        default:
            return "texture_null";
        }
    }

    /// <summary>
    /// Converts the texture type to a display-friendly string representation
    /// </summary>
    /// <param name="type">Texture type</param>
    /// <returns>Display-friendly string representation of the texture type</returns>
    static std::string TextureTypeToDisplayString(TextureType type)
    {
        switch (type)
        {
        case TextureType::DIFFUSE:
            return "Diffuse";
        case TextureType::AMBIENT:
            return "Ambient";
        case TextureType::SPECULAR:
            return "Specular";
        case TextureType::NORMAL:
            return "Normal";
        case TextureType::HEIGHT:
            return "Height";
        case TextureType::ALPHA:
            return "Alpha";
        default:
            return "Null";
        }
    }

    /// <summary>
    /// Generates a runtime texture from the given solid color
    /// </summary>
    /// <param name="color"></param>
    /// <param name="name"></param>
    /// <returns></returns>
    static unsigned int TextureFromColor(const glm::vec3& color, const std::string& name)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        unsigned char data[3] = { 0, 0, 0 };
        Vec3ToRGB(color, data);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return textureID;
    }

    /// <summary>
    /// Loads the given texture from a file
    /// </summary>
    /// <param name="directory">Directory of the texture</param>
    /// <param name="name">Filename of the texture</param>
    /// <param name="gamma">Whether to gamma correct the texture</param>
    /// <returns>OpenGL index of the texture</returns>
    static unsigned int TextureFromFile(const std::string& directory, const std::string& name, bool gamma = false)
    {
        std::string fileName = directory + '/' + name;
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width;
        int height;
        int nrComponents;
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

    /// <summary>
    /// Loads the given texture from a file
    /// </summary>
    /// <param name="directory">Directory of the texture</param>
    /// <param name="name">Filename of the texture</param>
    /// <param name="out_width">Return width of the texture</param>
    /// <param name="out_height">Return height of the texture</param>
    /// <param name="gamma">Whether to gamma correct the texture</param>
    /// <returns>OpenGL index of the texture</returns>
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
            std::cout << "Texture failed to load at path: " << fileName << std::endl;
            stbi_image_free(data);
        }
        return textureID;
    }

    /// <summary>
    /// Returns whether the texture is for internal use only
    /// </summary>
    /// <returns>Whether the texture is for internal use</returns>
    bool IsInternal()
    {
        return mIsInternal;
    }

	/// <summary>
	/// Creates a new instance of a texture that is already loaded into OpenGL
	/// </summary>
	/// <param name="_id">OpenGL textureID index</param>
	/// <param name="_type">Texture type</param>
	/// <param name="_path">Path of the referenced texture file</param>
	/// <param name="_name">Name of the texture</param>
	Texture(unsigned int _id = -1, TextureType _type = TextureType::DIFFUSE, const std::string& _path = "", const std::string& _name = "", bool _internal = false)
		: id(_id), type(_type), path(_path), name(_name), mIsInternal(_internal)
    {}

    /// <summary>
    /// Creates a new texture from the given color
    /// </summary>
    /// <param name="_type">Type of the texture</param>
    /// <param name="_color">Color of the texture</param>
    /// <param name="_name">Name of the texture</param>
    Texture(TextureType _type, const glm::vec3& _color, const std::string& _name, bool _internal = false)
        : type(_type), path("color"), name(_name), mIsInternal(_internal)
    {
        id = TextureFromColor(_color, _name);
    }

	/// <summary>
	/// Loads a new texture from the given file
	/// </summary>
	/// <param name="_type">Type of the texture</param>
	/// <param name="_directory">Directory of the texture file</param>
	/// <param name="_path">Name of the texture file</param>
	/// <param name="_name">Name of the texture</param>
	Texture(TextureType _type, const std::string& _directory, const std::string& _path, const std::string& _name, bool _internal = false)
		: type(_type), path(_path), name(_name), mIsInternal(_internal)
	{
		id = TextureFromFile(_directory, _path);
	}
};