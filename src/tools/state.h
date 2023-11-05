#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "rendering/material.h"
#include "interaction/actionStack.h"
#include "windows/guiVariableTypes.h"

class Selection;

class State
{
private:
	Config* mConfig;
	Selection* mSelection;
	ActionStack* mActionStack;

	std::unordered_map<std::string, Material*> mMaterialsThisFrame;
public:
	UIBool isDiscoLight;
	UIBool isRotatingLight;

	UIBool enableDiffuseMap;
	UIBool enableAmbientMap;
	UIBool enableSpecularMap;
	UIBool enableNormalMap;
	UIBool enableHeightMap;
	UIBool enableAlphaMap;
	UIBool enableShadows;
	UIBool enableGrid;
	UIBool drawByMaterial;
	UIBool flipTextures;
	UIBool drawGUI;

	UIInt DEBUG_fakeNumber;

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
		isDiscoLight = UIBool("Disco Light", false, mActionStack);
		isRotatingLight = UIBool("Rotating Light", false, mActionStack);

		enableDiffuseMap = UIBool("Enable Diffuse Map", true, mActionStack);
		enableAmbientMap = UIBool("Enable Ambient Map", true, mActionStack);
		enableSpecularMap = UIBool("Enable Specular Map", true, mActionStack);
		enableNormalMap = UIBool("Enable Normal Map", true, mActionStack);
		enableHeightMap = UIBool("Enable Height Map", true, mActionStack);
		enableAlphaMap = UIBool("Enable Alpha Map", true, mActionStack);
		enableShadows = UIBool("Enable Shadows", true, mActionStack);
		enableGrid = UIBool("Enabled Grid", true, mActionStack);
		drawByMaterial = UIBool("Draw By Material", true, mActionStack);
		flipTextures = UIBool("Flip Textures", true, mActionStack);
		drawGUI = UIBool("Draw UI", true, mActionStack);
		DEBUG_fakeNumber = UIInt("DEBUG number", 0, mActionStack);

		depthMapFBO = 0;
		depthMap = 0;
		depthMapSize = config->GetInt("quality.shadowResolution");
	}
};