#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "rendering/material.h"

class Selection;

class State
{
private:
	Config* mConfig;
	Selection* mSelection;

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

	Selection* GetSel() { return mSelection; }
	Config* GetConfig() { return mConfig; }

	State(Selection* selection, Config* config)
	{
		curShader = config->GetString("shader.file");
		mSelection = selection;
		mConfig = config;
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

		depthMapFBO = 0;
		depthMap = 0;
		depthMapSize = config->GetInt("quality.shadowResolution");
	}
};