#pragma once
#include "rendering/shader.h"
#include "interfaces/iContainer.h"

class ShaderContainer : public IContainer<Shader>
{
protected:
	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Shader*> ReadItem(std::ifstream& file) override
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

	}
};