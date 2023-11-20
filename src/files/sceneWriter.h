#pragma once
#include "timer.h"
#include "rendering/scene.h"
#include <iostream>
#include <fstream>

class SceneWriter
{
private:
	static void WriteNodes(Scene* scene, std::ofstream& file)
	{
		file << "# Nodes" << std::endl;
		Node::Write(file, scene->GetRootNode());
		file << std::endl;
	}

	static void WriteMeshes(Scene* scene, std::ofstream& file)
	{
		file << "# Meshes" << std::endl;
		scene->GetMeshes()->Write(file);
		file << std::endl;
	}

	static void WriteMaterials(Scene* scene, std::ofstream& file)
	{
		file << "# Materials" << std::endl;
		scene->GetMaterials()->Write(file);
		file << std::endl;
	}

	static void WriteTextures(Scene* scene, std::ofstream& file)
	{
		file << "# Textures" << std::endl;
		scene->GetTextures()->Write(file);
		file << std::endl;
	}

	static void WriteCameras(Scene* scene, std::ofstream& file)
	{
		file << "# Cameras" << std::endl;
		scene->GetCameras()->Write(file);
		file << std::endl;
	}

	static void WriteLights(Scene* scene, std::ofstream& file)
	{
		file << "# Lights" << std::endl;
		scene->GetLights()->Write(file);
		file << std::endl;
	}
public:
	static void SaveScene(Scene* scene, const std::string& path)
	{
		double startTime = glfwGetTime();
		std::ofstream file(path);
		try
		{
			WriteNodes(scene, file);
			WriteMeshes(scene, file);
			WriteMaterials(scene, file);
			WriteTextures(scene, file);
			WriteCameras(scene, file);
			WriteLights(scene, file);
			Timer::Time(startTime, "Save scene");
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::SAVESCENE::WRITE " << e.what() << std::endl;
		}
		file.close();
	}
};