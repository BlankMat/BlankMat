#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "rendering/material.h"
#include "interaction/actionStack.h"
#include "interaction/selection.h"
#include "windows/guiVariableTypes.h"

class State
{
private:
	Config* mConfig = nullptr;
	Selection* mSelection = nullptr;
	ActionStack* mActionStack = nullptr;

	std::string mCurFileName = "";
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

	unsigned int depthMapFBO = 0;
	unsigned int depthMap = 0;
	unsigned int depthMapSize = 1024;
	std::string fps = "";
	std::string frameTime = "";

	std::string curShader = "";
	Material* defaultMat = nullptr;

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

	// Returns the current filename
	std::string GetCurFileName() const
	{
		return mCurFileName;
	}

	// Sets the current filename
	void SetCurFileName(const std::string& fileName)
	{
		mCurFileName = fileName;
	}

	// Returns the selection object
	Selection* GetSel() const
	{
		return mSelection; 
	}

	// Returns the current config
	Config* GetConfig() const
	{
		return mConfig;
	}
	
	// Returns the action stack
	ActionStack* GetActionStack() const
	{
		return mActionStack;
	}

	// Saves the current state of the action stack
	void SaveActionStack()
	{
		mActionStack->Save();
	}

	// Returns whether the action stack is saved
	bool IsSaved() const
	{
		return mActionStack->IsSaved();
	}

	State(Config* config)
	{
		curShader = config->GetString("shader.file");
		mConfig = config;
		mSelection = new Selection();
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

		depthMapSize = config->GetInt("quality.shadowResolution");
	}
};