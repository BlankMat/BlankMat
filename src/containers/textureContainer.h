#pragma once
#include "rendering/texture.h"
#include "interfaces/iContainer.h"

class TextureContainer : public IContainer<Texture>
{
protected:
	/// <summary>
	/// Returns whether the item should be skipped or not
	/// </summary>
	/// <param name="item">Item to consider</param>
	/// <returns>Whether the item should be skipped</returns>
	bool SkipItem(Texture* item) override
	{
		return item->IsInternal();
	}

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Texture*> ReadItem(std::ifstream& file) override
	{
		return std::pair<std::string, Texture*>("", nullptr);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Texture* item, std::ofstream& file) override
	{
		file << "TEXTURE " << key << std::endl;
		file << "name " << item->name << std::endl;
		file << "path " << item->path << std::endl;
		file << "type " << Texture::TextureToTypeString(item->type) << std::endl;
	}

public:
	/// <summary>
	/// Adds the texture to the container, or if it already exists, updates the item to the given one
	/// </summary>
	/// <param name="item">The item to store</param>
	/// <returns>The element stored in the container</returns>
	Texture* AddMaterial(Texture* item)
	{
		return Add(item->name, item);
	}

	/// <summary>
	/// Initializes a default texture container with default textures
	/// </summary>
	explicit TextureContainer()
	{
		Add("default_diffuse", new Texture(TextureType::DIFFUSE, glm::vec3(1.0f), "default_diffuse", true));
		Add("default_ambient", new Texture(TextureType::AMBIENT, glm::vec3(1.0f), "default_ambient", true));
		Add("default_specular", new Texture(TextureType::SPECULAR, glm::vec3(1.0f), "default_specular", true));
		Add("default_normal", new Texture(TextureType::NORMAL, glm::vec3(0.5f, 0.5f, 1.0f), "default_normal", true));
		Add("default_height", new Texture(TextureType::HEIGHT, glm::vec3(1.0f), "default_height", true));
		Add("default_alpha", new Texture(TextureType::ALPHA, glm::vec3(1.0f), "default_alpha", true));
	}
};