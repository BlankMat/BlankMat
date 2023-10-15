#pragma once
#include "glIncludes.h"
#include "iScene.h"
#include "mesh.h"
#include "node.h"
#include "tools/state.h"

class Scene : public IScene
{
protected:
	std::string mDirectory;
	std::string mName;
	std::vector<Mesh*> mMeshList;
	Node* mRootNode;
	State* mState;

	void ProcessNode(Node* sceneNode, aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	// Loads the given model to the scene
	void LoadModel(const std::string& path);
	// Renders the current scene
	void Draw(Window* window) override;
	// Sets the shader for all the meshes of the model
	void SetMeshShaders(Shader* shader, State* state);
	// Saves the global state in the scene
	void SetState(State* state);
	// Adds the given node to the scene
	void AddNode(Node* node);
	// Adds the given mesh to the scene
	void AddMesh(Mesh* mesh);
	// Returns the root node
	Node* GetRootNode();
	// Constructs the scene, getting everything ready for manual setting
	Scene();
	// Destructs scene and cleans up all memory used
	~Scene();
};
