#pragma once
#include "iMaterial.h"
#include <string>

struct DataMaterial : public IMaterial<std::string>
{
	void UpdateShader(Shader* shader) override {}

	DataMaterial(glm::vec3 _ka = glm::vec3(), glm::vec3 _kd = glm::vec3(), glm::vec3 _ks = glm::vec3(),
		float _ns = 0, float _ni = 1, float _d = 1, glm::vec3 _ke = glm::vec3(), int _illum = 2)
		: IMaterial(_ka, _kd, _ks, _ns, _ni, _d, _ke, _illum) {}
};