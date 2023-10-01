#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "vertex.h"
#include "window.h"
#include "grid.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "selection.h"
#include <unordered_map>

#define GRID_OBJ "grid"
#define BG_PLANE_OBJ "bgPlane"
#define TRANSFORM_HANDLE "transformHandle"
#define CAMERA_AXIS_HANDLE "cameraAxisHandle"

class Scene
{
private:
	std::string mCurShader;
	Light* mGlobalLight;
	Camera* mMainCamera;
	Model* mCurModel;
	std::unordered_map<std::string, Entity*> mPreRenderList;
	std::unordered_map<std::string, Entity*> mRenderList;
	std::unordered_map<std::string, Shader*> mShaderList;
public:
	// Renders the current scene
	void Draw(Window* window);
	// Activates the shader with the given name for the scene
	void UseShader(std::string name);
	// Creates a shader for the scene with the given name from the source file of the given name
	void CreateShader(std::string name, bool loadGeom);
	// Creates a shader for the scene with the given name, loading it from a different source than the name
	void CreateShader(std::string name, std::string source, bool loadGeom);

	// Returns the scene's camera
	Camera* GetCamera() { return mMainCamera; }
	// Returns the scene's light
	Light* GetLight() { return mGlobalLight; }
	// Returns the scene's model
	Model* GetModel() { return mCurModel; }
	// Returns the shader with the given name
	Shader* GetShader(std::string name)
	{
		if (mShaderList.find(name) != mShaderList.end())
			return mShaderList[name];
		return nullptr;
	}
	// Returns the entity with the given name
	Entity* Getentity(std::string name)
	{
		if (mPreRenderList.find(name) != mPreRenderList.end())
			return mPreRenderList[name];
		else if (mRenderList.find(name) != mRenderList.end())
			return mRenderList[name];
		return nullptr;
	}

	// Sets up the scene's camera with the given options
	void SetCamera(Options* options) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = new Camera(options); }
	// Sets the scene's camera to the given camera
	void SetCamera(Camera* cam) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = cam; }
	// Sets the scene's light to the given light
	void SetLight(Light* light) { if (mGlobalLight != nullptr) { delete mGlobalLight; } mGlobalLight = light; }
	// Sets the scene's model to the given model
	void SetModel(Model* model) { if (mCurModel != nullptr) { delete mCurModel; } mCurModel = model; }
	// Adds an entity to the scene's render list
	void AddEntity(std::string name, Entity* entity, bool preRender = false) 
		{ (preRender ? mPreRenderList : mRenderList).emplace(name, entity); }

	// Returns the projection matrix of the scene's camera
	glm::mat4 GetProjectionMatrix(float aspect) { return GetCamera()->GetProjection(aspect); }
	// Returns the view matrix of the scene's camera
	glm::mat4 GetViewMatrix() { return GetCamera()->GetView(); }

	// Constructs the scene, getting everything ready for manual setting
	Scene();
	// Destructs scene and cleans up all memory used
	~Scene();
};
