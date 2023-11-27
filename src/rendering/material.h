#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "rendering/texture.h"
#include "rendering/shader.h"
#include "containers/textureContainer.h"

// Forward declare state to prevent circular dependency
class State;

// Lighting mode of the material
enum class IllumMode { FLAT = 0, LAMBERT = 1, PHONG = 2, REFLECTION = 3 };

/// <summary>
/// Class that stores all texture and render information for a material
/// </summary>
class Material
{
protected:
    glm::vec3 mCurKD = glm::vec3(1.0f);
    glm::vec3 mCurKA = glm::vec3(0.0f);
    glm::vec3 mCurKS = glm::vec3(1.0f);
    glm::vec3 mCurKE = glm::vec3(0.0f);
    bool mShadowsEnabled = true;
    bool mIsInternal = false;
    std::vector<Texture*> mCurTextures;
    std::vector<std::string> mCurTextureNames;

	/// <summary>
	/// List of textures that are part of this material
	/// </summary>
	std::vector<Texture*> mTextures;
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
    Texture* map_ka;
    /// <summary>
    /// diffuse color texture
    /// </summary>
    Texture* map_kd;
    /// <summary>
    /// specular color texture
    /// </summary>
    Texture* map_ks;
    /// <summary>
    /// bump/normal texture
    /// </summary>
    Texture* map_bump;
    /// <summary>
    /// specular highlight texture
    /// </summary>
    Texture* map_ns;
    /// <summary>
    /// alpha (dissolve) texture
    /// </summary>
    Texture* map_d;
    /// <summary>
    /// name of the material
    /// </summary>
    std::string name;

    /// <summary>
    /// Returns whether the material is for internal use only or not.
    /// </summary>
    /// <returns>Whether the material is internal</returns>
    bool IsInternal();

    /// <summary>
    /// Loads the textures of this material into the OpenGL context
    /// </summary>
    /// <param name="_state">Global state of the application</param>
    /// <param name="_defaultMat">Default material</param>
    void LoadTextures(State* _state, Material* _defaultMat);

    /// <summary>
    /// Updates the given shader with this material's properties
    /// </summary>
    /// <param name="_shader">Shader to use for this material</param>
    /// <returns>Index of next available GL texture</returns>
    unsigned int UpdateShader(Shader* _shader);

    /// <summary>
    /// Constructs the default material
    /// </summary>
    /// <param name="_textures">Texture container for the scene</param>
    explicit Material(TextureContainer* _textures);

    /// <summary>
    /// Constructs a material out of a single color (diffuse)
    /// </summary>
    /// <param name="_name">Name of the material</param>
    /// <param name="_color">Diffuse color of the material</param>
    /// <param name="_textures">Texture container for the scene</param>
    /// <param name="_internal">Whether the material is for internal use only</param>
    explicit Material(const std::string& _name, const glm::vec3& _color, TextureContainer* _textures, bool _internal = true);

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
    explicit Material(const std::string& _name, Config* _config, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
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
    explicit Material(const std::string& _name, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
        Texture* _map_bump, Texture* _map_ns, Texture* _map_d,
        const glm::vec3& _ka = glm::vec3(), const glm::vec3& _kd = glm::vec3(), const glm::vec3& _ks = glm::vec3(),
        float _ns = 0, float _ni = 1, float _d = 1, const glm::vec3& _ke = glm::vec3(), int _illum = 2);

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
    explicit Material(const std::string& _name, 
        const std::vector<Texture*>& _map_kd, const std::vector<Texture*>& _map_ka, const std::vector<Texture*>& _map_ks, 
        const std::vector<Texture*>& _map_bump, const std::vector<Texture*>& _map_ns, const std::vector<Texture*>& _map_d,
        const glm::vec3& _ka = glm::vec3(), const glm::vec3& _kd = glm::vec3(), const glm::vec3& _ks = glm::vec3(),
        float _ns = 0, float _ni = 1, float _d = 1, const glm::vec3& _ke = glm::vec3(), int _illum = 2);
};