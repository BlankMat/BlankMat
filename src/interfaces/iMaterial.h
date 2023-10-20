#pragma once
#include "glIncludes.h"
#include "rendering/shader.h"

enum class IllumMode { FLAT = 0, LAMBERT = 1, PHONG = 2, REFLECTION = 3 };

/// <summary>
/// Base material class that supports a template type for textures. Based on the Wavefront .obj/.mtl format.
/// </summary>
/// <typeparam name="T">Texture class type</typeparam>
template <typename T>
class IMaterial {
public:
	/// <summary>
	/// ambient color
	/// </summary>
	glm::vec3 ka;
	/// <summary>
	/// diffuse color
	/// </summary>
	glm::vec3 kd;
	/// <summary>
	/// specular color
	/// </summary>
	glm::vec3 ks;
	/// <summary>
	/// emissive color
	/// </summary>
	glm::vec3 ke;
	/// <summary>
	/// specular exponent
	/// </summary>
	float ns;
	/// <summary>
	/// index of refraction
	/// </summary>
	float ni;
	/// <summary>
	/// dissolve, AKA. transparency
	/// </summary>
	float d;
	/// <summary>
	/// Illumination mode
	/// </summary>
	int illum;

	/// <summary>
	/// ambient color texture
	/// </summary>
	T* map_ka;
	/// <summary>
	/// diffuse color texture
	/// </summary>
	T* map_kd;
	/// <summary>
	/// specular color texture
	/// </summary>
	T* map_ks;
	/// <summary>
	/// bump/normal texture
	/// </summary>
	T* map_bump;
	/// <summary>
	/// specular highlight texture
	/// </summary>
	T* map_ns;
	/// <summary>
	/// alpha (dissolve) texture
	/// </summary>
	T* map_d;
	/// <summary>
	/// name of the material
	/// </summary>
	std::string name;

	/// <summary>
	/// Constructs a material using the basic properties of a mtl material
	/// </summary>
	/// <param name="_name">Name of material</param>
	/// <param name="_ka"></param>
	/// <param name="_kd"></param>
	/// <param name="_ks"></param>
	/// <param name="_ns"></param>
	/// <param name="_ni"></param>
	/// <param name="_d"></param>
	/// <param name="_ke"></param>
	/// <param name="_illum"></param>
	IMaterial(std::string _name = "", glm::vec3 _ka = glm::vec3(), glm::vec3 _kd = glm::vec3(), glm::vec3 _ks = glm::vec3(),
		float _ns = 0, float _ni = 1, float _d = 1, glm::vec3 _ke = glm::vec3(), int _illum = 2)
		: name(_name), ka(_ka), kd(_kd), ks(_ks), ke(_ke), ns(_ns), ni(_ni), d(_d), illum(_illum)
	{
		map_ka = nullptr;
		map_kd = nullptr;
		map_ks = nullptr;
		map_bump = nullptr;
		map_ns = nullptr;
		map_d = nullptr;
	}
};