#pragma once
#include "glIncludes.h"
#include "blankMatConfig.h"
#include "rendering/scene.h"
#include <assimp/scene.h>
#include <iostream>
#include <fstream>

class SceneReader
{
private:
	// Recursively processes the meshes in the given node and all its children
	static void ProcessAssimpNode(const std::string& scope, Scene* scene, Node* node, const aiNode* assimpNode, const aiScene* assimpScene);

	// Process the vertices, indices, and textures of the given mesh
	static Mesh* ProcessAssimpMesh(const std::string& scope, Scene* scene, const aiMesh* assimpMesh, const aiScene* assimpScene);

	// Processes the vertices of the mesh, emplacing them into the list given
	static void ProcessAssimpVertices(const aiMesh* mesh, std::vector<Vertex>& outVertices);

	// Processes the indices of the mesh
	static void ProcessAssimpIndices(const std::vector<Vertex>& vertices, std::vector<unsigned int>& outIndices, const aiMesh* assimpMesh);

	// Processes the material of the mesh
	static Material* ProcessAssimpMaterial(const std::string& scope, Scene* scene, const aiMesh* assimpMesh, const aiScene* assimpScene);

	// Loads material textures
	static void LoadMaterialTextures(const std::string& scope, Scene* scene, std::vector<Texture*>& outTextures, const aiMaterial* assimpMaterial, aiTextureType assimpTextureType, TextureType type);

	static void ReadAssimpScene(Scene* scene, const std::string& path, bool replace);

	static void ReadBlankMatItem(const std::string& scope, IWritable* container, bool replace, std::ifstream& file, const std::string& label);

	static void ReadBlankMatScene(Scene* scene, const std::string& path, bool replace);

public:
	static void ClearScene(Scene* scene);

	static void LoadDefaultScene(Scene* scene);

	static void ReadScene(Scene* scene, const std::string& path, bool replace);
};