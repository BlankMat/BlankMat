#pragma once
#include "glIncludes.h"
#include "mesh.h"
#include "node.h"
#include "tools/state.h"
#include "interfaces/iScene.h"

class Scene : public IScene
{
protected:
	IEntity* mGrid;
	IEntity* mTransformHandle;
	IEntity* mViewAxisHandle;

	int mViewAxisPrevWidth;
	int mViewAxisPrevHeight;
	glm::mat4 mViewAxisProjection;

	// Returns the projection matrix of the view axis handle
	const glm::mat4& GetViewAxisProjection(Window* window);
public:
	// Loads the given model to the scene
	void LoadModel(const std::string& path, const glm::vec3& startPos = glm::vec3(0.0f), const glm::vec3& startRot = glm::vec3(0.0f), const glm::vec3& startScale = glm::vec3(1.0f));
	// Renders the current scene
	void Draw(Window* window, Shader* shader) override;
	// Renders the current scene's shadows
	void DrawShadows(Window* window, Shader* shader);

	// Sets the view axis handle to the given object
	void SetViewAxisHandle(IEntity* viewHandle);
	// Sets the grid object
	void SetGrid(IEntity* grid);
	// Sets the transform handle object
	void SetTransformHandle(IEntity* handle);

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
