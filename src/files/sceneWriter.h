#pragma once
#include "timer.h"
#include "rendering/scene.h"
#include <iostream>
#include <fstream>
#include <thread>

class SceneWriter
{
private:
	static void WriteItem(IWritable* item, std::ofstream& file, const std::string& label)
	{
		double startTime = Timer::Start();
		file << "# " << label << std::endl;
		item->Write(file);
		file << std::endl;
		Timer::Time(startTime, "Wrote " + label);
	}
public:
	static void SaveScene(Scene* scene, const std::string& path)
	{
		double startTime = Timer::Start();
		std::ofstream file(path);
		try
		{
			WriteItem(scene->GetRootNode(), file, "Nodes");
			WriteItem(scene->GetMeshes(), file, "Meshes");
			WriteItem(scene->GetMaterials(), file, "Materials");
			WriteItem(scene->GetTextures(), file, "Textures");
			WriteItem(scene->GetCameras(), file, "Cameras");
			WriteItem(scene->GetLights(), file, "Lights");
			Timer::Time(startTime, "Saved scene");
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::SAVESCENE::WRITE " << e.what() << std::endl;
		}
		file.close();
	}
};