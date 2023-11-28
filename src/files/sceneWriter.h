#pragma once
#include "timer.h"
#include "rendering/scene.h"
#include <iostream>
#include <fstream>
#include <thread>

class SceneWriter
{
private:
	static void WriteNodes(Scene* scene, std::ofstream& file)
	{
		double startTime = glfwGetTime();
		file << "# Nodes" << std::endl;
		Node::Write(file, scene->GetRootNode());
		file << std::endl;
		Timer::Time(startTime, "Wrote nodes");
	}

	static void WriteMeshes(Scene* scene, std::ofstream& file)
	{
		double startTime = glfwGetTime();
		file << "# Meshes" << std::endl;
		scene->GetMeshes()->Write(file);
		file << std::endl;
		Timer::Time(startTime, "Wrote meshes");
	}

	static void WriteMaterials(Scene* scene, std::ofstream& file)
	{
		double startTime = glfwGetTime();
		file << "# Materials" << std::endl;
		scene->GetMaterials()->Write(file);
		file << std::endl;
		Timer::Time(startTime, "Wrote materials");
	}

	static void WriteTextures(Scene* scene, std::ofstream& file)
	{
		double startTime = glfwGetTime();
		file << "# Textures" << std::endl;
		scene->GetTextures()->Write(file);
		file << std::endl;
		Timer::Time(startTime, "Wrote textures");
	}

	static void WriteCameras(Scene* scene, std::ofstream& file)
	{
		double startTime = glfwGetTime();
		file << "# Cameras" << std::endl;
		scene->GetCameras()->Write(file);
		file << std::endl;
		Timer::Time(startTime, "Wrote cameras");
	}

	static void WriteLights(Scene* scene, std::ofstream& file)
	{
		double startTime = glfwGetTime();
		file << "# Lights" << std::endl;
		scene->GetLights()->Write(file);
		file << std::endl;
		Timer::Time(startTime, "Wrote lights");
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