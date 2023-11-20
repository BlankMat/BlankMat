#pragma once
#include "rendering/light.h"
#include "interfaces/iContainer.h"

class LightContainer : public IContainer<Light>
{
protected:
	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Light*> ReadItem(std::ifstream& file) override
	{
		return std::pair<std::string, Light*>("", nullptr);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Light* item, std::ofstream& file) override
	{

	}
};