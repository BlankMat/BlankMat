#pragma once
#include "rendering/mesh.h"
#include "interfaces/iContainer.h"

class MeshContainer : public IContainer<Mesh>
{
protected:
	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Mesh*> ReadItem(std::ifstream& file) override
	{
		return std::pair<std::string, Mesh*>("", nullptr);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Mesh* item, std::ofstream& file) override
	{

	}
};