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
	}
};