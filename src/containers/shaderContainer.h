#pragma once
#include "rendering/shader.h"
#include "interfaces/iContainer.h"

class ShaderContainer : public IContainer<Shader>
{
protected:
	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	void RenameItem(Shader* item, const std::string& name) override {}

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="scope">Scope to read item in</param>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Shader*> ReadItem(const std::string& scope, std::ifstream& file) override
	{
		return std::pair<std::string, Shader*>("", nullptr);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Shader* item, std::ofstream& file) override
	{
		file << "SHADER " << key << std::endl;
	}
};