#pragma once
#include "glIncludes.h"
#include "indexStructs.h"
#include "ray.h"
#include "scene.h"

class RayTracer
{
public:
	static Ray GenerateRay(Scene* scene, float u, float v, bool print = false);
	static glm::vec3 Bisector(glm::vec3 a, glm::vec3 b);
	static glm::vec3 Normal(std::vector<glm::vec3>& vPos);
	static glm::vec3 GetPixelColor(Scene* scene, Ray& r, int count);
	static void Raytrace(Scene* scene, std::vector<glm::vec4>& result);
};