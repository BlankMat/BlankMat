#pragma once
#include "glIncludes.h"
#include "triangle.h"
#include "itriangle.h"
#include "indexStructs.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

	float GetT(ITriangle& tri);
	bool IntersectTriangle(ITriangle& tri);
	ITriangle GetClosestTriangle(std::vector<ITriangle>& tris);
	IndVertex GetClosestVertex(std::vector<ITriangle>& tris);
	int GetNumIntersects(std::vector<ITriangle>& tris);
	bool DoesIntersect(std::vector<ITriangle>& tris);

	Ray(glm::vec3 origin, glm::vec3 direction);
};