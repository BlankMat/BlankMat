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
	std::vector<Mesh> meshes;
	std::vector<Texture> texturesLoaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model(char* path);
	void draw(Shader& shader);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);