#include "material.h"

void Material::Reset()
{
	ka = glm::vec3();
	kd = glm::vec3();
	ks = glm::vec3();
	ns = 0.0f;
	ni = 1.0f;
	d = 1.0f;
	mapkd = "";
}

// Constructor for material
Material::Material(glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks, float _ns, float _ni, float _d, std::string _mapkd)
	: ka(_ka), kd(_kd), ks(_ks), ns(_ns), ni(_ni), d(_d), mapkd(_mapkd) {}