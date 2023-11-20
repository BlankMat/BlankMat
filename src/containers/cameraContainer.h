#pragma once
#include "rendering/camera.h"
#include "interfaces/iContainer.h"

class CameraContainer : public IContainer<Camera>
{
protected:
	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Camera*> ReadItem(std::ifstream& file) override
	{
		return std::pair<std::string, Camera*>("", nullptr);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Camera* item, std::ofstream& file) override
	{
		file << "CAMERA " << key << std::endl;
		file << "fov " << item->GetFOV() << std::endl;
		file << "nearclip " << item->GetNearClip() << std::endl;
		file << "farclip " << item->GetFarClip() << std::endl;
		file << "pos " << Vec3ToString(item->GetPos()) << std::endl;
		file << "dir " << Vec3ToString(item->GetDir()) << std::endl;
		file << "up " << Vec3ToString(item->GetUp()) << std::endl;
		file << "bgcolor " << Vec3ToString(item->GetBGColor()) << std::endl;
		file << "orthsize " << item->GetOrthSize() << std::endl;
		file << "perspective " << (int)item->IsPerspective() << std::endl;
		file << "wireframe " << (int)item->IsWireframe() << std::endl;
	}
public:
};