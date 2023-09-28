#pragma once
#include "glIncludes.h"

class Drawable {
public:
	virtual void Draw(glm::mat4 viewProj) = 0;
};