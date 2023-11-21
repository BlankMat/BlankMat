#pragma once
#include "timer.h"
#include "rendering/scene.h"
#include <iostream>
#include <fstream>

class SceneReader
{
private:
	static void ReadNodes(Scene* scene, std::ifstream& file)
	{
		scene->AddNode(Node::Read(file, scene->GetRootNode()));
	}

	static void ReadMeshes(Scene* scene, std::ifstream& file)
	{
		scene->GetMeshes()->Read(file);
	}

	static void ReadMaterials(Scene* scene, std::ifstream& file)
	{
		scene->GetMaterials()->Read(file);
	}

	static void ReadTextures(Scene* scene, std::ifstream& file)
	{
		scene->GetTextures()->Read(file);
	}

	static void ReadCameras(Scene* scene, std::ifstream& file)
	{
		scene->GetCameras()->Read(file);
	}

	static void ReadLights(Scene* scene, std::ifstream& file)
	{
		scene->GetLights()->Read(file);
	}
public:
	static void ReadScene(Scene* scene, const std::string& path)
	{
		double startTime = glfwGetTime();
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cout << "ERROR::READSCENE::FILE_DOES_NOT_EXIST file " << path << " could not be opened." << std::endl;
			return;
		}

		try
		{
			ReadNodes(scene, file);
			ReadMeshes(scene, file);
			ReadMaterials(scene, file);
			ReadTextures(scene, file);
			ReadCameras(scene, file);
			ReadLights(scene, file);
			Timer::Time(startTime, "Read scene");	
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::READSCENE::READ " << e.what() << std::endl;
		}
		file.close();
	}
};