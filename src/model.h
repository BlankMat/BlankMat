#pragma once
#include "glIncludes.h"
#include "drawable.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include <vector>
#include <string>

class Model : public Drawable
{
protected:
	int mDefaultTextureIndex = -1;
	std::vector<Mesh> mMeshList;
	std::vector<Texture> mLoadedTextureList;
	std::string mDirectory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadDefaultTexture();
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void GenBuffers() override {}
public:
	Model(std::string path);
	void Draw(glm::mat4 viewProj);
	void SetMeshShaders(Shader* shader);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);