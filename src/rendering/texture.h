#pragma once
#include "glIncludes.h"
#include "utils.h"
#include "interfaces/iSelectable.h"
#include <string>
#include <iostream>
#include <stb_image.h>

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
struct Texture : public ISelectable
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
	unsigned int mID;

	/// <summary>
	/// The filename of the texture
	/// </summary>
	std::string mFile;

	/// <summary>
	/// The path of the directory for the texture
	/// </summary>
	std::string mDir;

	/// <summary>
	/// The type of the texture
	/// </summary>
	TextureType mType;

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
    static unsigned int TextureFromColor(const glm::vec3& color)
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
    /// <param name="filename">Filename of the texture</param>
    /// <param name="gamma">Whether to gamma correct the texture</param>
    /// <returns>OpenGL index of the texture</returns>
    static unsigned int TextureFromFile(const std::string& directory, const std::string& filename)
    {
        int w, h;
        return TextureFromFile(directory, filename, w, h);
    }

    /// <summary>
    /// Loads the given texture from a file
    /// </summary>
    /// <param name="directory">Directory of the texture</param>
    /// <param name="filename">Filename of the texture</param>
    /// <param name="out_width">Return width of the texture</param>
    /// <param name="out_height">Return height of the texture</param>
    /// <param name="gamma">Whether to gamma correct the texture</param>
    /// <returns>OpenGL index of the texture</returns>
    static unsigned int TextureFromFile(const std::string& directory, const std::string& filename, int& outWidth, int& outHeight)
    {
        std::string fileName = directory + '/' + filename;
        unsigned int textureID;
        glGenTextures(1, &textureID);
        int numComponents;
        unsigned char* data = stbi_load(fileName.c_str(), &outWidth, &outHeight, &numComponents, 0);
        if (data)
        {
            GLenum format = GL_RGB;
            if (numComponents == 1)
                format = GL_RED;
            else if (numComponents == 3)
                format = GL_RGB;
            else if (numComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, outWidth, outHeight, 0, format, GL_UNSIGNED_BYTE, data);
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
    /// Creates a new texture from the given color
    /// </summary>
    /// <param name="name">Name of the texture</param>
    /// <param name="scope">Scope of the texture</param>
    /// <param name="type">Type of the texture</param>
    /// <param name="color">Color of the texture</param>
	/// <param name="internal">Whether the texture is internal only</param>
    Texture(const std::string& name, const std::string& scope, TextureType type, const glm::vec3& color, bool internal = false)
        : mType(type), mDir(""), mFile(""), mIsInternal(internal), ISelectable(SelectableType::TEXTURE)
    {
        InitName(name, scope);
        mID = TextureFromColor(color);
    }

	/// <summary>
	/// Loads a new texture from the given file
	/// </summary>
    /// <param name="name">Name of the texture</param>
    /// <param name="scope">Scope of the texture</param>
    /// <param name="type">Type of the texture</param>
	/// <param name="dir">Path of the referenced texture file</param>
	/// <param name="filename">Name of the texture</param>
	/// <param name="internal">Whether the texture is internal only</param>
	Texture(const std::string& name, const std::string& scope, TextureType type, const std::string& dir, const std::string& filename, bool internal = false)
		: mType(type), mDir(dir), mFile(filename), mIsInternal(internal), ISelectable(SelectableType::TEXTURE)
	{
        InitName(name, scope);
        mID = TextureFromFile(dir, filename);
	}

    /// <summary>
    /// Creates a new instance of a texture that is already loaded into OpenGL
    /// </summary>
    /// <param name="name">Name of the texture</param>
    /// <param name="scope">Scope of the texture</param>
    /// <param name="id">OpenGL textureID index</param>
    /// <param name="type">Texture type</param>
    /// <param name="dir">Path of the referenced texture file</param>
    /// <param name="filename">Name of the texture</param>
    /// <param name="internal">Whether the texture is internal only</param>
    Texture(const std::string& name = "", const std::string& scope = "", unsigned int id = -1, TextureType type = TextureType::DIFFUSE,
        const std::string& dir = "", const std::string& filename = "", bool internal = false)
        : mID(id), mType(type), mDir(dir), mFile(filename), mIsInternal(internal), ISelectable(SelectableType::TEXTURE)
    {
        InitName(name, scope);
    }
};