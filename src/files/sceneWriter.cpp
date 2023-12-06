#include "files/sceneWriter.h"
#include "timer.h"
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>
#include <thread>

void SceneWriter::WriteBlankMatItem(IWritable* item, std::ofstream& file, const std::string& label)
{
	double startTime = Timer::Start();
	file << "# " << label << std::endl;
	item->Write(file);
	file << std::endl;
	Timer::Time(startTime, "Wrote " + std::to_string(item->WriteCount()) + " " + label);
}

void SceneWriter::SaveAssimpScene(Scene* scene, const std::string& path)
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

void SceneWriter::SaveBlankMatScene(Scene* scene, const std::string& path)
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

void SceneWriter::SaveScene(Scene* scene, const std::string& path)
{
	// Get extension
	std::string ext = GetFileExtension(path);
	if (ext == BLANK)
	{
		SaveBlankMatScene(scene, path);
	}
	else
	{
		SaveAssimpScene(scene, path);
	}
}