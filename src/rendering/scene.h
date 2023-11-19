#pragma once
#include "glIncludes.h"
#include "mesh.h"
#include "node.h"
#include "interfaces/iScene.h"
#include "tools/state.h"

class Scene : public IScene
{
protected:
	IEntity* mGrid = nullptr;
	IEntity* mTransformHandle = nullptr;
	IEntity* mViewAxisHandle = nullptr;

	int mViewAxisPrevWidth = 0;
	int mViewAxisPrevHeight = 0;
	glm::mat4 mViewAxisProjection = glm::mat4(1.0f);

	// Returns the projection matrix of the view axis handle
	const glm::mat4& GetViewAxisProjection(Window* window);
public:
	// Renders the current scene
	void Draw(Window* window, Shader* shader) override;
	// Renders the current scene's shadows
	void DrawShadows(Window* window, Shader* shader);

	// Focuses the camera on the currently selected object (if any)
	void FocusCamera();

	// Toggles the camera between perspective and orthographic
	void TogglePerspective();

	// Toggles the rotation mode of the camera
	void ToggleCameraRotationMode();

	// Sets the view axis handle to the given object
	void SetViewAxisHandle(IEntity* viewHandle);
	// Sets the grid object
	void SetGrid(IEntity* grid);
	// Sets the transform handle object
	void SetTransformHandle(IEntity* handle);

	// Constructs the scene, getting everything ready for manual setting
	Scene(State* state);
	// Destructs scene and cleans up all memory used
	~Scene();
};
