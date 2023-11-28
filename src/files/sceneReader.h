#pragma once
#include "timer.h"
#include "rendering/scene.h"
#include <iostream>
#include <fstream>

class SceneReader
{
private:
	static void ReadItem(IWritable* item, std::ifstream& file, const std::string& label)
	{
		double startTime = Timer::Start();
		item->Read(file);
		Timer::Time(startTime, "Read " + label);
	}
public:
	static void ReadScene(Scene* scene, const std::string& path)
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
			ReadItem(scene->GetRootNode(), file, "Nodes");
			ReadItem(scene->GetMeshes(), file, "Meshes");
			ReadItem(scene->GetMaterials(), file, "Materials");
			ReadItem(scene->GetTextures(), file, "Textures");
			ReadItem(scene->GetCameras(), file, "Cameras");
			ReadItem(scene->GetLights(), file, "Lights");
			Timer::Time(startTime, "Read scene");
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::READSCENE::READ " << e.what() << std::endl;
		}
		file.close();
	}
};