#pragma once
#include "rendering/dataMaterial.h"

// Storage of all options for the program
struct Options {
	// Object
	std::string objName;
	float objScale;
	glm::vec3 objPos;

	// Light
	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 lightColor;
	float lightKA;
	float lightKS;
	bool isDiscoLight;
	bool isRotatingLight;
	bool gamma;

	DataMaterial defaultMat;
	glm::vec3 bgColor;
	std::string shader;
	bool wireframe;
	bool shaderGeom;

	// Camera options
	glm::vec3 camPos;
	glm::vec3 camLookAt;
	glm::vec3 camUp;
	float camFov;
	float camNearClip;
	float camFarClip;
	float camSize;
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