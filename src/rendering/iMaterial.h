#pragma once
#include "glIncludes.h"
#include "shader.h"

template <typename T>
struct IMaterial {
	glm::vec3 ka;			// ambient color
	glm::vec3 kd;			// diffuse color
	glm::vec3 ks;			// specular color
	glm::vec3 ke;			// emissive color
	float ns;				// specular exponent
	float ni;				// index of refraction
	float d;				// dissolve, AKA. transparency
	int illum;				// Illumination mode

	T map_ka;				// ambient color texture
	T map_kd;				// diffuse color texture
	T map_ks;				// specular color texture
	T map_bump;				// bump/normal texture
	T map_ns;				// specular highlight texture
	T map_d;				// alpha (dissolve) texture

	// Updates the given shader with the material's values
	virtual void UpdateShader(Shader* shader) = 0;

	IMaterial(glm::vec3 _ka = glm::vec3(), glm::vec3 _kd = glm::vec3(), glm::vec3 _ks = glm::vec3(),
		float _ns = 0, float _ni = 1, float _d = 1, glm::vec3 _ke = glm::vec3(), int _illum = 2)
		: ka(_ka), kd(_kd), ks(_ks), ke(_ke), ns(_ns), ni(_ni), d(_d), illum(_illum) {}
};