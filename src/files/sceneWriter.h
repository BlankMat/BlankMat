#pragma once
#include "blankMatConfig.h"
#include "rendering/scene.h"
#include <assimp/scene.h>
#include <iostream>
#include <fstream>

class SceneWriter
{
private:
	static std::string GetAssimpFormatID(const Assimp::Exporter& exporter, const std::string& path);

	// Processes the given mesh
	static unsigned int ProcessAssimpMesh(Mesh* sourceMesh, aiMesh*& newMesh, std::vector<aiMesh*>& outMeshWriteList, std::vector<aiMaterial*>& outMatList);

	// Processes all materials of the scene
	static void ProcessAssimpMaterials(Scene* sourceScene, aiScene*& newScene, std::vector<aiMaterial*>& outMatList, std::vector<aiTexture*>& outTextureList);

	// Processes all textures of the scene
	static void ProcessAssimpTextures(Scene* sourceScene, const std::string& path, std::vector<aiTexture*>& outTextureList);

	// Recursively processes the meshes in the given node and all its children
	static void ProcessAssimpNode(Scene* sourceScene, Node* sourceNode, aiNode*& newNode, aiScene*& newScene, std::vector<aiMesh*>& outMeshWriteList, std::vector<aiMaterial*>& outMatList);

	static void GenerateAssimpScene(Scene* scene, aiScene*& assimpScene, const std::string& path);

	static void SaveAssimpScene(Scene* scene, const std::string& path);

	static void WriteBlankMatItem(IWritable* item, std::ofstream& file, const std::string& label);

	static void SaveBlankMatScene(Scene* scene, const std::string& path);
public:
	static void SaveScene(Scene* scene, const std::string& path);
};