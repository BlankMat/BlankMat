#pragma once
#include "glIncludes.h"
#include "iEntity.h"
#include "texture.h"
#include "shader.h"
#include "modelMesh.h"
#include <vector>
#include <string>

class Model : public IEntity
{
protected:
	int mDefaultTextureIndex = -1;
	std::vector<ModelMesh*> mMeshList;
	std::vector<Texture> mLoadedTextureList;
	std::string mDirectory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	ModelMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadDefaultTexture();
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void GenBuffers() override {}
public:
	Model(std::string path);
	~Model();
	void Draw(glm::mat4 viewProj);
	void SetMeshShaders(Shader* shader);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);