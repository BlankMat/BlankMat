#pragma once
#include "glIncludes.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;

	Vertex(glm::vec3 _pos = glm::vec3(), glm::vec3 _normal = glm::vec3(), glm::vec2 _texCoords = glm::vec2(), glm::vec3 _tangent = glm::vec3())
		: pos(_pos), normal(_normal), texCoords(_texCoords), tangent(_tangent) {}
};