#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "selection.h"
#include "interfaces/iScene.h"

class Selection;

class ITool
{
protected:
	bool mIsSelected;
	//Selection* mSel;
	
public:
	virtual void Select(State* state, IScene* scene, Window* window) = 0;
	virtual void Deselect(State* state, IScene* scene, Window* window) = 0;
	virtual void Interact(State* state, IScene* scene, Window* window) = 0;
};