#pragma once
#include "interfaces/iEntity.h"
#include "interfaces/iContainer.h"

class EntityContainer : public IContainer<IEntity>
{
protected:
	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	void RenameItem(IEntity* item, const std::string& name) override
	{
		item->SetName(name);
	}

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="scope">Scope to read item in</param>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, IEntity*> ReadItem(const std::string& scope, std::ifstream& file) override
	{
		return std::pair<std::string, IEntity*>("", nullptr);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, IEntity* item, std::ofstream& file) override
	{
		file << "ENTITY " << key << std::endl;
	}
public:
	// Draws the contents of the container
	void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats = false)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			iter->second->Draw(shader, state, defaultMat, viewProj, drawMats);
		}
	}
};