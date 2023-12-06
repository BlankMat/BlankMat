#pragma once
#include "blankMatConfig.h"
#include "rendering/scene.h"
#include <assimp/scene.h>
#include <iostream>
#include <fstream>

class SceneWriter
{
private:
	static void WriteBlankMatItem(IWritable* item, std::ofstream& file, const std::string& label);

	static void SaveAssimpScene(Scene* scene, const std::string& path);

	static void SaveBlankMatScene(Scene* scene, const std::string& path);
public:
	static void SaveScene(Scene* scene, const std::string& path);
};