#pragma once
#include "glIncludes.h"

struct Light 
{
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 color;
	float ka;
	float ks;

	virtual void Draw(glm::mat4 viewProj);

	Light(glm::vec3 _pos = glm::vec3(1.0f), glm::vec3 _dir = glm::vec3(-1.0f), glm::vec3 _color = glm::vec3(1.0f), 
		float _ka = 0.1f, float _ks = 0.5f);
};