#pragma once
#include "glIncludes.h"
#include "iScene.h"
#include "mesh.h"
#include "node.h"

class Scene : public IScene
{
protected:
	std::string mDirectory;
	std::vector<Mesh*> mMeshList;
	Node* mRootNode;

	void ProcessNode(Node* sceneNode, aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	// Loads the given model to the scene
	void LoadModel(std::string path);
	// Renders the current scene
	void Draw(Window* window) override;
	// Sets the shader for all the meshes of the model
	void SetMeshShaders(Shader* shader);
	// Returns the root node
	Node* GetRootNode() { return mRootNode; }
	// Constructs the scene, getting everything ready for manual setting
	Scene();
	// Destructs scene and cleans up all memory used
	~Scene();
};
