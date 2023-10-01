#pragma once
#include "glIncludes.h"
#include "shader.h"
#include <string>
#include <vector>

struct Material {
	glm::vec3 ka;			// ambient color
	glm::vec3 kd;			// diffuse color
	glm::vec3 ks;			// specular color
	glm::vec3 ke;			// emissive color
	float ns;				// specular exponent
	float ni;				// index of refraction
	float d;				// dissolve, AKA. transparency
	int illum;				// Illumination mode

	std::string map_ka;		// ambient color texture
	std::string map_kd;		// diffuse color texture
	std::string map_ks;		// specular color texture
	std::string map_bump;	// bump/normal texture
	std::string map_ns;		// specular highlight texture
	std::string map_d;		// alpha (dissolve) texture

	// Updates the given shader with the material's values
	void UpdateShader(Shader* shader)
	{
		shader->SetVec3("material.ambient", ka);
		shader->SetVec3("material.diffuse", kd);
		shader->SetVec3("material.specular", ks);
		shader->SetFloat("material.shininess", ns);
	}

	Material(glm::vec3 _ka = glm::vec3(), glm::vec3 _kd = glm::vec3(), glm::vec3 _ks = glm::vec3(),
		float _ns = 0, float _ni = 1, float _d = 1, glm::vec3 _ke = glm::vec3(),
		std::string _map_kd = "default", std::string _map_ka = "default", std::string _map_ks = "default", std::string _map_bump = "default",
		std::string _map_ns = "default", std::string _map_d = "default", int _illum = 2)
		: ka(_ka), kd(_kd), ks(_ks), ke(_ke), ns(_ns), ni(_ni), d(_d), illum(_illum), 
		map_ka(_map_ka), map_kd(_map_kd), map_ks(_map_ks), map_bump(_map_bump), map_ns(_map_ns), map_d(_map_d) {}

	static Material Average(std::vector<Material*>& _mats)
	{
		// Init values
		float n = 1.0f / _mats.size();
		glm::vec3 _ka = glm::vec3();
		glm::vec3 _kd = glm::vec3();
		glm::vec3 _ks = glm::vec3();
		float _ns = 0.0f;
		float _ni = 0.0f;
		float _d = 0.0f;

		// Add values
		for (int i = 0; i < _mats.size(); i++) {
			_ka += _mats[i]->ka;
			_kd += _mats[i]->kd;
			_ks += _mats[i]->ks;
			_ns += _mats[i]->ns;
			_ni += _mats[i]->ni;
			_d += _mats[i]->d;
		}

		// Average values
		return Material(_ka * n, _kd * n, _ks * n, _ns * n, _ni * n, _d * n);
	}
};