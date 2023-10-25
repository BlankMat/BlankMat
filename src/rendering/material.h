#pragma once
#include "texture.h"
#include "files/config.h"
#include "interfaces/iMaterial.h"

// Forward declare state to prevent circular dependency
class State;

/// <summary>
/// Class that stores all texture and render information for a material
/// </summary>
class Material : public IMaterial<Texture>
{
protected:
	/// <summary>
	/// List of textures that are part of this material
	/// </summary>
	std::vector<Texture*> mTextures;
public:
    /// <summary>
    /// Updates the given shader with this material's properties
    /// </summary>
    /// <param name="_shader">Shader to use for this material</param>
    /// <param name="_state">Global state of the app</param>
    /// <param name="_defaultMat">Default material for the app</param>
    /// <returns>Index of next available GL texture</returns>
    unsigned int UpdateShader(Shader* _shader, State* _state, Material* _defaultMat);

    /// <summary>
    /// Constructs a material out of a single color (diffuse)
    /// </summary>
    /// <param name="_color">Diffuse color of the material</param>
    /// <param name="_name">Name of the material</param>
    Material(const glm::vec3& _color, const std::string& _name = "");

    /// <summary>
    /// Constructs a material out of a config file and preloaded textures
    /// </summary>
    /// <param name="_name">Name of the material</param>
    /// <param name="_config">Config to build material from</param>
    /// <param name="_map_kd">Diffuse texture</param>
    /// <param name="_map_ka">Ambient texture</param>
    /// <param name="_map_ks">Specular texture</param>
    /// <param name="_map_bump">Normal/Bump texture</param>
    /// <param name="_map_ns">Specular highlight/Height texture</param>
    /// <param name="_map_d">Alpha texture</param>
    Material(const std::string& _name, Config* _config, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
        Texture* _map_bump, Texture* _map_ns, Texture* _map_d);

    /// <summary>
    /// Constructs a material out of preloaded textures
    /// </summary>
    /// <param name="_name">Name of the material</param>
    /// <param name="_map_kd">Diffuse texture</param>
    /// <param name="_map_ka">Ambient texture</param>
    /// <param name="_map_ks">Specular texture</param>
    /// <param name="_map_bump">Normal/Bump texture</param>
    /// <param name="_map_ns">Specular highlight/Height texture</param>
    /// <param name="_map_d">Alpha texture</param>
    /// <param name="_ka">Alpha color</param>
    /// <param name="_kd">Diffuse color</param>
    /// <param name="_ks">Specular color</param>
    /// <param name="_ns">Specular exponent</param>
    /// <param name="_ni">Index of refraction</param>
    /// <param name="_d">Alpha</param>
    /// <param name="_ke">Emissive color</param>
    /// <param name="_illum">Illumination mode</param>
    Material(const std::string& _name, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
        Texture* _map_bump, Texture* _map_ns, Texture* _map_d,
        const glm::vec3& _ka = glm::vec3(), const glm::vec3& _kd = glm::vec3(), const glm::vec3& _ks = glm::vec3(),
        const float _ns = 0, const float _ni = 1, const float _d = 1,
        const glm::vec3& _ke = glm::vec3(), const int _illum = 2);

    /// <summary>
    /// Constructs a material out of lists of preloaded textures
    /// </summary>
    /// <param name="_name">Name of the material</param>
    /// <param name="_map_kd">Diffuse textures</param>
    /// <param name="_map_ka">Ambient textures</param>
    /// <param name="_map_ks">Specular textures</param>
    /// <param name="_map_bump">Normal/Bump textures</param>
    /// <param name="_map_ns">Specular highlight/Height textures</param>
    /// <param name="_map_d">Alpha textures</param>
    /// <param name="_ka">Alpha color</param>
    /// <param name="_kd">Diffuse color</param>
    /// <param name="_ks">Specular color</param>
    /// <param name="_ns">Specular exponent</param>
    /// <param name="_ni">Index of refraction</param>
    /// <param name="_d">Alpha</param>
    /// <param name="_ke">Emissive color</param>
    /// <param name="_illum">Illumination mode</param>
    Material(const std::string& _name, 
        const std::vector<Texture*>& _map_kd, const std::vector<Texture*>& _map_ka, const std::vector<Texture*>& _map_ks, 
        const std::vector<Texture*>& _map_bump, const std::vector<Texture*>& _map_ns, const std::vector<Texture*>& _map_d,
        const glm::vec3& _ka = glm::vec3(), const glm::vec3& _kd = glm::vec3(), const glm::vec3& _ks = glm::vec3(),
        const float _ns = 0, const float _ni = 1, const float _d = 1,
        const glm::vec3& _ke = glm::vec3(), const int _illum = 2);
};