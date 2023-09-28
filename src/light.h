#pragma once
#include "glIncludes.h"

struct Light 
{
	glm::vec3 pos;
	glm::vec3 offset;
	glm::vec3 dir;
	glm::vec3 color;
	float ka;
	float ks;

	virtual void Draw(glm::mat4 viewProj) {}

	Light(glm::vec3 _pos = glm::vec3(1.0f), glm::vec3 _dir = glm::vec3(-1.0f), glm::vec3 _color = glm::vec3(1.0f), 
		float _ka = 0.1f, float _ks = 0.5f) : pos(_pos), dir(_dir), color(_color), ka(_ka), ks(_ks), offset(_pos) {}
};