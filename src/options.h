#pragma once
#include "fileHelper.h"
#include "material.h"

// Storage of all options for the program
struct Options {
	std::string objName;
	float objScale;
	glm::vec3 objPos;
	Material defaultColor;
	int vertexModel;
	int wireframe;
	int print;
	std::string shader;
	bool shaderGeom;

	// Camera options
	glm::vec3 camPos;
	glm::vec3 camLookAt;
	glm::vec3 camUp;
	float camFov;
	float camNearClip;
	float camFarClip;
	glm::vec2 camSize;
	bool isPerspective;
};

// A structure containing constants of movement and rotation
struct SpeedConsts {
	float cameraMoveSpeed = 3.0f;
	float cameraTurnSpeed = 1.0f;

	float modelMoveSpeed = 3.0f;
	float modelTurnSpeed = 5.0f;
	float modelScaleRate = 1.0f;

	float mouseMoveSpeed = 0.1f;
	float mouseTurnSpeed = 0.1f;

	SpeedConsts(float _camMove, float _camTurn, float _modelMove, float _modelTurn, float _modelScale, float _mouseMove, float _mouseTurn);
};

Options ReadOptions(std::string fileName);