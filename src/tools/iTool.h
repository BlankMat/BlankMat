#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "selection.h"

class ITool
{
protected:
	bool mIsSelected;
	Selection* mSel;
	
public:
	virtual void Select() = 0;
	virtual void Deselect() = 0;
	virtual void Interact(double cursorX, double cursorY) = 0;
};