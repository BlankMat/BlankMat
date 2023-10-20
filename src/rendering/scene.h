#pragma once
#include "glIncludes.h"
#include "mesh.h"
#include "node.h"
#include "tools/state.h"
#include "interfaces/iScene.h"

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
	void LoadModel(const std::string& path, glm::vec3 startPos = glm::vec3(0.0f), glm::vec3 startRot = glm::vec3(0.0f), glm::vec3 startScale = glm::vec3(1.0f));
	// Renders the current scene
	void Draw(Window* window, Shader* shader) override;
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
