#pragma once
#include "rendering/texture.h"
#include "interfaces/iContainer.h"

class TextureContainer : public IContainer<Texture>
{
protected:
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
		file << "type " << item->type << std::endl;
	}
};