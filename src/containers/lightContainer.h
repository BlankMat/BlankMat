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
		file << "LIGHT " << key << std::endl;
		file << "type " << (int)item->GetType() << std::endl;
		file << "pos " << Vec3ToString(item->GetPos()) << std::endl;
		file << "dir " << Vec3ToString(item->GetDir()) << std::endl;
		file << "color " << Vec3ToString(item->GetColor()) << std::endl;
		file << "kd " << item->GetKD() << std::endl;
		file << "ka " << item->GetKA() << std::endl;
		file << "ks " << item->GetKS() << std::endl;
		file << "gamma " << item->GetGamma() << std::endl;
		file << "range " << item->GetRange() << std::endl;
		file << "spotinner " << item->GetSpotInnerRadius() << std::endl;
		file << "spotouter " << item->GetSpotOuterRadius() << std::endl;
	}
};