#pragma once
#include "glIncludes.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "mathLib.h"
#include <vector>
#include <string>

class Model
{
private:
	unsigned int defaultTextureIndex = -1;
	std::vector<Mesh> meshes;
	std::vector<Texture> texturesLoaded;
	std::string directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadDefaultTexture();
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model(std::string path);
	void Draw(Shader& shader);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);