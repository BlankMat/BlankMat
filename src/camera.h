#pragma once
#include "options.h"

struct Camera
{
	float fov;
	float nearClip;
	float farClip;

	glm::vec3 pos;
	glm::vec3 rotation;

	glm::vec3 dir;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec2 orthSize;
	bool isPerspective;

	void CalcBasis();
	void SetFromOptions(Options* options);

	Camera(float _fov = -45.0f, float _nearClip = 0.1f, float _farClip = 100.0f, 
		glm::vec3 _pos = glm::vec3(-2, 0, 0), glm::vec3 _dir = glm::vec3(0, 0, 0), glm::vec3 _up = glm::vec3(0, 1, 0), 
		glm::vec2 _orthSize = glm::vec2(), bool _isPerspective = true);
	Camera(Options* options);
};