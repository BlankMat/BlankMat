#pragma once
#include "rendering/material.h"
#include "interfaces/iContainer.h"
#include "containers/textureContainer.h"

class MaterialContainer : public IContainer<Material>
{
protected:
	/// <summary>
	/// Returns whether the item should be skipped or not
	/// </summary>
	/// <param name="item">Item to consider</param>
	/// <returns>Whether the item should be skipped</returns>
	bool SkipItem(Material* item) override
	{
		return item->IsInternal();
	} 
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
		file << "MATERIAL " << key << std::endl;
		file << "kd " << Vec3ToString(item->kd) << std::endl;
		file << "ka " << Vec3ToString(item->ka) << std::endl;
		file << "ks " << Vec3ToString(item->ks) << std::endl;
		file << "ke " << Vec3ToString(item->ke) << std::endl;
		file << "ns " << item->ns << std::endl;
		file << "ni " << item->ni << std::endl;
		file << "d " << item->d << std::endl;
		file << "illum " << item->illum << std::endl;

		// Write textures only if they are used
		if (item->map_kd != nullptr)
			file << "map_kd " << item->map_kd->name << std::endl;
		if (item->map_ka != nullptr)
			file << "map_ka " << item->map_ka->name << std::endl;
		if (item->map_ks != nullptr)
			file << "map_ks " << item->map_ks->name << std::endl;
		if (item->map_bump != nullptr)
			file << "map_bump " << item->map_bump->name << std::endl;
		if (item->map_ns != nullptr)
			file << "map_ns " << item->map_ns->name << std::endl;
		if (item->map_d != nullptr)
			file << "map_d " << item->map_d->name << std::endl;
	}

public:
	/// <summary>
	/// Adds the material to the container, or if it already exists, updates the item to the given one
	/// </summary>
	/// <param name="item">The item to store</param>
	/// <returns>The element stored in the container</returns>
	Material* AddMaterial(Material* item)
	{
		return Add(item->name, item);
	}

	/// <summary>
	/// Returns the default material
	/// </summary>
	/// <returns>Default material</returns>
	Material* GetDefault()
	{
		return GetItem("default");
	}

	/// <summary>
	/// Initializes a default material container with the default material
	/// </summary>
	/// <param name="textures">Texture container with default textures</param>
	explicit MaterialContainer(TextureContainer* textures)
	{
		Add("default", new Material(textures));
	}
};