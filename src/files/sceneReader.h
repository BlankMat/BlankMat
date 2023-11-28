#pragma once
#include "timer.h"
#include "rendering/scene.h"
#include <iostream>
#include <fstream>

class SceneReader
{
private:
	static void ReadItem(IWritable* item, bool replace, std::ifstream& file, const std::string& label)
	{
		std::cout << "Reading " << label << std::endl;
		double startTime = Timer::Start();
		item->Read(file, replace);
		Timer::Time(startTime, "Read " + label);
	}
public:
	static void ReadScene(Scene* scene, const std::string& path, bool replace)
	{
		double startTime = Timer::Start();
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cout << "ERROR::READSCENE::FILE_DOES_NOT_EXIST file " << path << " could not be opened." << std::endl;
			return;
		}

		try
		{
			ReadItem(scene->GetRootNode(), replace, file, "Nodes");
			ReadItem(scene->GetMeshes(), replace, file, "Meshes");
			ReadItem(scene->GetMaterials(), replace, file, "Materials");
			ReadItem(scene->GetTextures(), replace, file, "Textures");
			ReadItem(scene->GetCameras(), replace, file, "Cameras");
			ReadItem(scene->GetLights(), replace, file, "Lights");
			Timer::Time(startTime, "Read scene");
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::READSCENE::READ " << e.what() << std::endl;
		}
		file.close();
	}
};