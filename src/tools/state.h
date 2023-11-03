#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "rendering/material.h"
#include "interaction/actionStack.h"

class Selection;

class State
{
private:
	Config* mConfig;
	Selection* mSelection;
	ActionStack* mActionStack;

	std::unordered_map<std::string, Material*> mMaterialsThisFrame;
public:
	bool isDiscoLight;
	bool isRotatingLight;

	bool enableDiffuseMap;
	bool enableAmbientMap;
	bool enableSpecularMap;
	bool enableNormalMap;
	bool enableHeightMap;
	bool enableAlphaMap;
	bool enableShadows;
	bool enableGrid;
	bool drawByMaterial;
	bool flipTextures;
	bool drawGUI;

	int DEBUG_fakeNumber;

	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int depthMapSize;
	std::string fps;
	std::string frameTime;

	std::string curShader;
	Material* defaultMat;

	// Loads the given material, unless it's already been loaded this frame
	void LoadMaterial(Material* material)
	{
		if (mMaterialsThisFrame.find(material->name) != mMaterialsThisFrame.end())
			return;

		material->LoadTextures(this, defaultMat);
		mMaterialsThisFrame.emplace(material->name, material);
	}

	// Clears the loaded materials from this frame
	void ClearLoadedMaterials()
	{
		mMaterialsThisFrame.clear();
	}

	// Returns the selection object
	Selection* GetSel() 
	{
		return mSelection; 
	}

	// Returns the current config
	Config* GetConfig()
	{
		return mConfig;
	}
	
	// Returns the action stack
	ActionStack* GetActionStack()
	{
		return mActionStack;
	}

	State(Selection* selection, Config* config)
	{
		curShader = config->GetString("shader.file");
		mSelection = selection;
		mConfig = config;
		mActionStack = new ActionStack();
		isDiscoLight = false;
		isRotatingLight = false;

		enableDiffuseMap = true;
		enableAmbientMap = true;
		enableSpecularMap = true;
		enableNormalMap = true;
		enableHeightMap = true;
		enableAlphaMap = true;
		enableShadows = true;
		enableGrid = true;
		drawByMaterial = true;
		flipTextures = true;
		drawGUI = true;
		DEBUG_fakeNumber = 0;

		depthMapFBO = 0;
		depthMap = 0;
		depthMapSize = config->GetInt("quality.shadowResolution");
	}
};