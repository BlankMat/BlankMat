#pragma once
#include "glIncludes.h"
#include "timer.h"
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
	Timer* mTimer = nullptr;

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
	UIBool enableDefaultMat;
	UIBool enableGrid;
	UIBool drawByMaterial;
	UIBool flipTextures;
	UIBool drawGUI;
	UIBool collapseMeshNodes;
	UIBool collapseScope;
	UIBool expandAllNodes;

	unsigned int depthMapFBO = 0;
	unsigned int depthMap = 0;
	unsigned int depthMapSize = 1024;

	// Frame information
	int mousePrevX = 0;
	int mousePrevY = 0;
	int mouseCurX = 0;
	int mouseCurY = 0;
	bool shouldRender = true;

	std::string curShader = "";
	Material* defaultMat = nullptr;

	// Loads the given material, unless it's already been loaded this frame
	void LoadMaterial(Material* material)
	{
		if (mMaterialsThisFrame.contains(material->GetScopedName()))
			return;

		material->LoadShaderTextures(this, defaultMat);
		mMaterialsThisFrame.emplace(material->GetScopedName(), material);
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

	// Returns the timer
	Timer* GetTimer() const
	{
		return mTimer;
	}

	// Saves the current state of the action stack
	void SaveActionStack()
	{
		mActionStack->Save();
	}

	// Clears the action stack
	void ClearActionStack()
	{
		mActionStack->Clear();
	}

	// Returns whether the action stack is saved
	bool IsSaved() const
	{
		return mActionStack->IsSaved();
	}

	// Toggles the GUI display
	void ToggleDisplayGUI()
	{
		drawGUI.Set(!drawGUI);
	}

	// Toggles the grid display
	void ToggleDisplayGrid()
	{
		enableGrid.Set(!enableGrid);
	}

	// Toggles the diffuse maps
	void ToggleDiffuseMap()
	{
		enableDiffuseMap.Set(!enableDiffuseMap);
	}

	// Toggles the ambient maps
	void ToggleAmbientMap()
	{
		enableAmbientMap.Set(!enableAmbientMap);
	}

	// Toggles the specular maps
	void ToggleSpecularMap()
	{
		enableSpecularMap.Set(!enableSpecularMap);
	}

	// Toggles the normal maps
	void ToggleNormalMap()
	{
		enableNormalMap.Set(!enableNormalMap);
	}

	// Toggles the height maps
	void ToggleHeightMap()
	{
		enableHeightMap.Set(!enableHeightMap);
	}

	// Toggles the alpha maps
	void ToggleAlphaMap()
	{
		enableAlphaMap.Set(!enableAlphaMap);
	}

	// Toggles shadow display
	void ToggleShadows()
	{
		enableShadows.Set(!enableShadows);
	}

	// Toggles default material display
	void ToggleDefaultMat()
	{
		enableDefaultMat.Set(!enableDefaultMat);
	}

	// Toggles collapsing of mesh nodes
	void ToggleCollapseMeshNodes()
	{
		collapseMeshNodes.Set(!collapseMeshNodes);
	}

	// Toggles collapsing of scope
	void ToggleCollapseScope()
	{
		collapseScope.Set(!collapseScope);
	}

	// Toggles expanding of all nodes
	void ToggleExpandAllNodes()
	{
		expandAllNodes.Set(!expandAllNodes);
	}

	State(Config* config)
	{
		curShader = config->GetString("shader.file");
		mConfig = config;
		mSelection = new Selection();
		mActionStack = new ActionStack();
		mTimer = new Timer();

		isDiscoLight = UIBool("Disco Light", false, mActionStack);
		isRotatingLight = UIBool("Rotating Light", false, mActionStack);

		enableDiffuseMap = UIBool("Enable Diffuse Map", true, mActionStack);
		enableAmbientMap = UIBool("Enable Ambient Map", true, mActionStack);
		enableSpecularMap = UIBool("Enable Specular Map", true, mActionStack);
		enableNormalMap = UIBool("Enable Normal Map", true, mActionStack);
		enableHeightMap = UIBool("Enable Height Map", true, mActionStack);
		enableAlphaMap = UIBool("Enable Alpha Map", true, mActionStack);
		enableShadows = UIBool("Enable Shadows", true, mActionStack);
		enableDefaultMat = UIBool("Enable Default Material", true, mActionStack);
		enableGrid = UIBool("Enabled Grid", true, mActionStack);
		drawByMaterial = UIBool("Draw By Material", true, mActionStack);
		flipTextures = UIBool("Flip Textures", true, mActionStack);
		drawGUI = UIBool("Draw UI", true, mActionStack);
		collapseMeshNodes = UIBool("Collapse Mesh Nodes", true, mActionStack);
		collapseScope = UIBool("Collapse Scope", true, mActionStack);
		expandAllNodes = UIBool("Expand All Nodes", false, mActionStack);

		depthMapSize = config->GetInt("quality.shadowResolution");
	}
};