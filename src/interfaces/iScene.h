#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "windows/window.h"
#include "rendering/shader.h"
#include "rendering/light.h"
#include "rendering/camera.h"
#include "rendering/material.h"
#include "interfaces/iEntity.h"
#include <unordered_map>

class IScene
{
protected:
	std::string mCurShader;
	Shader* mShader;
	Light* mGlobalLight;
	Camera* mMainCamera;
	Material* mDefaultMat;

	std::unordered_map<std::string, Light*> mLightList;
	std::unordered_map<std::string, Camera*> mCameraList;
	std::unordered_map<std::string, Shader*> mShaderList;
	std::unordered_map<std::string, Material*> mMaterialList;
	std::unordered_map<std::string, Texture*> mTextureList;
	std::unordered_map<std::string, IEntity*> mPreRenderList;
	std::unordered_map<std::string, IEntity*> mRenderList;

	// Loads the material of the given config. Must be bottom-level config
	Material* LoadMaterial(Config* config, std::string name)
	{
		// Don't handle null configs
		if (config == nullptr)
			return nullptr;

		// If the material exists already, return it
		Material* material = GetMaterial(name);
		if (material != nullptr)
			return material;

		// Create textures
		Texture* kd = LoadTexture("texture_diffuse", config->GetString("map_kd"), "default_diffuse");
		Texture* ka = LoadTexture("texture_ambient", config->GetString("map_ka"), "default_ambient");
		Texture* ks = LoadTexture("texture_specular", config->GetString("map_ks"), "default_specular");
		Texture* normal = LoadTexture("texture_normal", config->GetString("map_bump"), "default_normal");
		Texture* ns = LoadTexture("texture_height", config->GetString("map_ns"), "default_height");
		Texture* d = LoadTexture("texture_alpha", config->GetString("map_d"), "default_alpha");

		// Create material
		material = new Material(name, config, kd, ka, ks, normal, ns, d);
		AddMaterial(name, material);
		return material;
	}

	// Loads the given texture or returns the existing one
	Texture* LoadTexture(std::string type, std::string path, std::string defaultName = "")
	{
		// If no path is given, load the default texture
		if (path == "")
			path = defaultName;

		// Get the name of the texture
		std::string name = path.substr(0, path.find_last_of('.'));
		if (name == "default")
			name = defaultName;

		// If the texture is already loaded with the same type, return it
		Texture* texture = GetTexture(name);
		if (texture != nullptr && texture->type == type)
			return texture;

		// If the texture is not loaded, load the texture and store it
		texture = new Texture(type, FileSystem::GetPath(TEXTURE_DIR), path, name);
		AddTexture(name, texture);
		return texture;
	}

public:
	// Renders the current scene
	virtual void Draw(Window* window, Shader* shader) = 0;

	// Activates the shader with the given name for the scene
	void UseShader(std::string name = "")
	{
		if (name == "")
			name = mCurShader;
		if (mShaderList.find(name) != mShaderList.end() && mShaderList[name] != nullptr)
		{
			mCurShader = name;
			mShader = mShaderList[name];
			mShader->Use();
		}
	}

	// Creates a shader for the scene with the given name from the source file of the given name
	Shader* CreateShader(std::string name, bool loadGeom)
	{
		if (mShaderList.find(name) == mShaderList.end())
		{
			Shader* newShader = new Shader(name, loadGeom);
			newShader->name = name;
			mShaderList.emplace(name, newShader);
			return newShader;
		}
		return mShaderList[name];
	}

	// Creates a shader for the scene with the given name, loading it from a different source than the name
	Shader* CreateShader(std::string name, std::string source, bool loadGeom)
	{
		if (mShaderList.find(name) == mShaderList.end())
		{
			Shader* newShader = new Shader(source, loadGeom);
			newShader->name = name;
			mShaderList.emplace(name, newShader);
			return newShader;
		}
		return mShaderList[name];
	}

	// Creates a shader for the scene with the given name, loading it from a config
	Shader* CreateShader(std::string name, Config* config)
	{
		if (mShaderList.find(name) == mShaderList.end())
		{
			Shader* newShader = new Shader(config->GetString("file"), config->GetBool("hasGeomShader"));
			newShader->name = name;
			mShaderList.emplace(name, newShader);
			return newShader;
		}
		return mShaderList[name];
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

	// Returns the material with the given name
	Material* GetMaterial(std::string name)
	{
		if (mMaterialList.find(name) != mMaterialList.end())
			return mMaterialList[name];
		return nullptr;
	}

	// Returns the default material
	Material* GetDefaultMat()
	{
		return mDefaultMat;
	}

	// Returns the material with the given name
	Texture* GetTexture(std::string name)
	{
		if (mTextureList.find(name) != mTextureList.end())
			return mTextureList[name];
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
	
	// Returns a reference to the shader list
	std::unordered_map<std::string, Shader*>& GetShaderList() { return mShaderList; }

	// Returns a reference to the material list
	std::unordered_map<std::string, Material*>& GetMaterialList() { return mMaterialList; }

	// Returns a reference to the texture list
	std::unordered_map<std::string, Texture*>& GetTextureList() { return mTextureList; }

	// Returns the current shader
	std::string GetCurShader() { return mCurShader; }

	// Sets up the scene's camera with the given options
	void SetCamera(Config* config) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = new Camera(config); }

	// Sets the scene's camera to the given camera
	void SetCamera(Camera* cam) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = cam; }

	// Sets the scene's light to the given light
	void SetLight(Light* light) { if (mGlobalLight != nullptr) { delete mGlobalLight; } mGlobalLight = light; }

	// Adds an entity to the scene's render list
	IEntity* AddEntity(IEntity* entity, bool preRender = false)
	{
		std::unordered_map<std::string, IEntity*>& tempList = (preRender ? mPreRenderList : mRenderList);
		if (tempList.find(entity->GetName()) == tempList.end())
			tempList.emplace(entity->GetName(), entity);
		return entity;
	}

	// Adds an entity to the scene's render list
	IEntity* AddEntity(IEntity* entity, std::string name, bool preRender = false)
	{
		std::unordered_map<std::string, IEntity*>& tempList = (preRender ? mPreRenderList : mRenderList);
		if (tempList.find(name) == tempList.end())
			tempList.emplace(name, entity);
		return entity;
	}

	// Adds a texture to the scene's texture list
	Texture* AddTexture(std::string name, Texture* texture)
	{
		if (mTextureList.find(name) == mTextureList.end())
			mTextureList.emplace(name, texture);
		return texture;
	}

	// Adds a material to the scene's material list
	Material* AddMaterial(std::string name, Material* material)
	{
		if (mMaterialList.find(name) == mMaterialList.end())
			mMaterialList.emplace(name, material);
		return material;
	}

	// Sets the default material of the scene from the material list if the material exists.
	Material* SetDefaultMaterial(std::string name)
	{
		if (mMaterialList.find(name) != mMaterialList.end())
			mDefaultMat = mMaterialList[name];
		return mDefaultMat;
	}

	// Sets the default material of the scene. Note: Does not add it to the material list.
	Material* SetDefaultMaterial(Material* material)
	{
		mDefaultMat = material;
		return mDefaultMat;
	}

	// Loads the materials from the given config
	void LoadMaterials(Config* config)
	{
		// Load the default material if it exists
		LoadMaterial(config->GetConfig("default"), "default");

		// Load the rest of the materials
		std::unordered_map<std::string, Config*> configs = config->GetConfigs();
		for (auto iter = configs.begin(); iter != configs.end(); ++iter)
			if (GetMaterial(iter->first) == nullptr)
				LoadMaterial(iter->second, iter->first);
	}

	// Returns the projection matrix of the scene's camera
	glm::mat4 GetProjectionMatrix(float aspect) { return GetCamera()->GetProjection(aspect); }

	// Returns the view matrix of the scene's camera
	glm::mat4 GetViewMatrix() { return GetCamera()->GetView(); }
};
