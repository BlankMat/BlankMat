#pragma once
#include "timer.h"
#include "rendering/scene.h"
#include <iostream>
#include <fstream>
#include <thread>

class SceneWriter
{
private:
	static void WriteBlankMatItem(IWritable* item, std::ofstream& file, const std::string& label)
	{
		double startTime = Timer::Start();
		file << "# " << label << std::endl;
		item->Write(file);
		file << std::endl;
		Timer::Time(startTime, "Wrote " + label);
	}

	static void SaveAssimpScene(Scene* scene, const std::string& path)
	{
		double startTime = Timer::Start();
		std::cout << "Saving scene to file " << path << std::endl;
		std::ofstream file(path);
		try
		{
			Timer::Time(startTime, "Saved scene");
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::EXPORTSCENE::WRITE " << e.what() << std::endl;
		}
		file.close();
	}

	static void SaveBlankMatScene(Scene* scene, const std::string& path)
	{
		double startTime = Timer::Start();
		std::cout << "Saving scene to file " << path << std::endl;
		std::ofstream file(path);
		try
		{
			WriteBlankMatItem(scene->GetRootNode(), file, "Nodes");
			WriteBlankMatItem(scene->GetMeshes(), file, "Meshes");
			WriteBlankMatItem(scene->GetMaterials(), file, "Materials");
			WriteBlankMatItem(scene->GetTextures(), file, "Textures");
			WriteBlankMatItem(scene->GetCameras(), file, "Cameras");
			WriteBlankMatItem(scene->GetLights(), file, "Lights");
			Timer::Time(startTime, "Saved scene");
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::SAVESCENE::WRITE " << e.what() << std::endl;
		}
		file.close();
	}
public:
	static void SaveScene(Scene* scene, const std::string& path)
	{
		// Get extension
		size_t lastPeriod = path.find_last_of('.');
		std::string ext = "";
		if (lastPeriod < path.length())
			ext = path.substr(lastPeriod);

		if (ext == ".blank")
		{
			SaveBlankMatScene(scene, path);
		}
		else
		{
			SaveAssimpScene(scene, path);
		}
	}
};