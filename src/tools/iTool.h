#pragma once
#include "glIncludes.h"

class ITool
{
protected:
	bool mIsSelected;
public:
	virtual void Select() = 0;
	virtual void Deselect() = 0;
	virtual void Interact() = 0;
};