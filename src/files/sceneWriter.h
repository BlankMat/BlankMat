#pragma once
#include "rendering/scene.h"
#include <iostream>
#include <fstream>

class SceneWriter
{
private:
	static void WriteNodes(Scene* scene, std::ofstream& file)
	{
		file << "# Nodes" << std::endl;
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
		std::ofstream file(path);
		WriteNodes(scene, file);
		WriteMeshes(scene, file);
		WriteMaterials(scene, file);
		WriteCameras(scene, file);
		WriteLights(scene, file);
		file.close();
	}
};