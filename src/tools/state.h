#pragma once
#include "glIncludes.h"
#include "files/config.h"

class Selection;

class State
{
private:
	Config* mConfig;
	Selection* mSelection;
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

	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int depthMapSize;
	std::string fps;
	std::string frameTime;

	std::string curShader;

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

		depthMapFBO = 0;
		depthMap = 0;
		depthMapSize = config->GetInt("quality.shadowResolution");
	}
};