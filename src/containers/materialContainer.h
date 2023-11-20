#pragma once
#include "rendering/material.h"
#include "interfaces/iContainer.h"

class MaterialContainer : public IContainer<Material>
{
protected:
	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Material*> ReadItem(std::ifstream& file) override
	{
		return std::pair<std::string, Material*>("", nullptr);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Material* item, std::ofstream& file) override
	{

	}
};