#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "light.h"
#include "camera.h"
#include "material.h"
#include "rendering/iEntity.h"
#include "files/config.h"
#include "windows/window.h"
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
	std::unordered_map<std::string, Texture*> mTextureList;

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
		bool isDefault = (name == "default");
		Texture* kd = LoadTexture("texture_diffuse", config->GetString("map_kd"), isDefault ? "default_diffuse" : "");
		Texture* ka = LoadTexture("texture_ambient", config->GetString("map_ka"), isDefault ? "default_ambient" : "");
		Texture* ks = LoadTexture("texture_specular", config->GetString("map_ks"), isDefault ? "default_specular" : "");
		Texture* normal = LoadTexture("texture_normal", config->GetString("map_bump"), isDefault ? "default_normal" : "");
		Texture* ns = LoadTexture("texture_height", config->GetString("map_ns"), isDefault ? "default_height" : "");
		Texture* d = LoadTexture("texture_alpha", config->GetString("map_d"), isDefault ? "default_alpha" : "");

		// Create material
		material = new Material(name, config, kd, ka, ks, normal, ns, d);
		AddMaterial(name, material);
		return material;
	}

	// Loads the given texture or returns the existing one
	Texture* LoadTexture(std::string type, std::string path, std::string name = "")
	{
		// If the texture is already loaded with the same type, return it
		Texture* texture = GetTexture(path);
		if (texture != nullptr && texture->type == type)
			return texture;

		// Otherwise load the texture and store it
		name = (name != "") ? name : path;
		texture = new Texture(type, FileSystem::GetPath(TEXTURE_DIR), path, name);
		AddTexture(name, texture);
		return texture;
	}

public:
	// Renders the current scene
	virtual void Draw(Window* window) = 0;

	// Activates the shader with the given name for the scene
	void UseShader(std::string name = "")
	{
		if (name == "")
			name = mCurShader;
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

	// Creates a shader for the scene with the given name, loading it from a config
	void CreateShader(std::string name, Config* config)
	{
		if (mShaderList.find(name) == mShaderList.end())
			mShaderList.emplace(name, new Shader(config->GetString("file"), config->GetBool("hasGeomShader")));
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

	// Returns the current shader
	std::string GetCurShader() { return mCurShader; }

	// Sets up the scene's camera with the given options
	void SetCamera(Config* config) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = new Camera(config); }

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
