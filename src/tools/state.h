#pragma once
#include "glIncludes.h"
#include "selection.h"
#include "files/config.h"

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
	}
};