#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "window.h"
#include "light.h"
#include "camera.h"
#include "material.h"
#include <unordered_map>

class IScene
{
protected:
	std::string mCurShader;
	Light* mGlobalLight;
	Camera* mMainCamera;

	std::unordered_map<std::string, IEntity*> mPreRenderList;
	std::unordered_map<std::string, IEntity*> mRenderList;
	std::unordered_map<std::string, Shader*> mShaderList;
	std::unordered_map<std::string, Material*> mMaterialList;
public:
	// Renders the current scene
	virtual void Draw(Window* window) = 0;

	// Activates the shader with the given name for the scene
	void UseShader(std::string name)
	{
		if (mShaderList.find(name) != mShaderList.end() && mShaderList[name] != nullptr)
		{
			mShaderList[name]->Use();
			mCurShader = name;
		}
	}

	// Creates a shader for the scene with the given name from the source file of the given name
	void CreateShader(std::string name, bool loadGeom)
	{
		if (mShaderList.find(name) == mShaderList.end())
			mShaderList.emplace(name, new Shader(name, loadGeom));
	}

	// Creates a shader for the scene with the given name, loading it from a different source than the name
	void CreateShader(std::string name, std::string source, bool loadGeom)
	{
		if (mShaderList.find(name) == mShaderList.end())
			mShaderList.emplace(name, new Shader(source, loadGeom));
	}

	// Returns the scene's camera
	Camera* GetCamera() { return mMainCamera; }

	// Returns the scene's light
	Light* GetLight() { return mGlobalLight; }

	// Returns the shader with the given name
	Shader* GetShader(std::string name)
	{
		if (mShaderList.find(name) != mShaderList.end())
			return mShaderList[name];
		return nullptr;
	}

	// Returns the entity with the given name
	IEntity* GetEntity(std::string name)
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

	// Adds an entity to the scene's render list
	IEntity* AddEntity(std::string name, IEntity* entity, bool preRender = false)
	{
		std::unordered_map<std::string, IEntity*>* tempList = &(preRender ? mPreRenderList : mRenderList);
		if (tempList->find(name) == tempList->end())
			tempList->emplace(name, entity);
		return entity;
	}

	// Adds a material to the scene's matreial list
	Material* AddMaterial(std::string name, Material* material)
	{
		if (mMaterialList.find(name) == mMaterialList.end())
			mMaterialList.emplace(name, material);
		return material;
	}

	// Returns the projection matrix of the scene's camera
	glm::mat4 GetProjectionMatrix(float aspect) { return GetCamera()->GetProjection(aspect); }

	// Returns the view matrix of the scene's camera
	glm::mat4 GetViewMatrix() { return GetCamera()->GetView(); }
};
