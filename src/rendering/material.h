#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "interfaces/iSelectable.h"
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
class Material : public ISelectable
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

    std::string mTargetMapKD = "default_diffuse";
    std::string mTargetMapKA = "default_ambient";
    std::string mTargetMapKS = "default_specular";
    std::string mTargetMapBump = "default_normal";
    std::string mTargetMapNS = "default_height";
    std::string mTargetMapD = "default_alpha";

	/// <summary>
	/// List of textures that are part of this material
	/// </summary>
	std::vector<Texture*> mTextures;
public:
    /// <summary>
    /// Ambient color
    /// </summary>
    glm::vec3 mKA;
    /// <summary>
    /// Diffuse color
    /// </summary>
    glm::vec3 mKD;
    /// <summary>
    /// Specular color
    /// </summary>
    glm::vec3 mKS;
    /// <summary>
    /// Emissive color
    /// </summary>
    glm::vec3 mKE;
    /// <summary>
    /// Specular exponent
    /// </summary>
    float mNS;
    /// <summary>
    /// Index of Refraction
    /// </summary>
    float mNI;
    /// <summary>
    /// Dissolve, AKA. transparency
    /// </summary>
    float mD;
    /// <summary>
    /// Illumination mode
    /// </summary>
    int mIllum;

    /// <summary>
    /// Diffuse color texture
    /// </summary>
    Texture* mMapKD;
    /// <summary>
    /// Ambient color texture
    /// </summary>
    Texture* mMapKA;
    /// <summary>
    /// Specular color texture
    /// </summary>
    Texture* mMapKS;
    /// <summary>
    /// Bump/Normal texture
    /// </summary>
    Texture* mMapBump;
    /// <summary>
    /// Specular highlight texture
    /// </summary>
    Texture* mMapNS;
    /// <summary>
    /// Alpha (dissolve) texture
    /// </summary>
    Texture* mMapD;

    /// <summary>
    /// Returns whether the material is for internal use only or not.
    /// </summary>
    /// <returns>Whether the material is internal</returns>
    bool IsInternal();

    /// <summary>
    /// Updates the given shader with this material's properties
    /// </summary>
    /// <param name="shader">Shader to use for this material</param>
    /// <returns>Index of next available GL texture</returns>
    unsigned int UpdateShader(Shader* shader);

    /// <summary>
    /// Loads the textures of this material into the OpenGL context
    /// </summary>
    /// <param name="state">Global state of the application</param>
    /// <param name="defaultMat">Default material</param>
    void LoadShaderTextures(State* state, Material* defaultMat);

    /// <summary>
    /// Loads the textures of the material from the scene's texture list
    /// </summary>
    /// <param name="textures">Texture container for the scene</param>
    void LoadMaterialTextures(TextureContainer* textures);

    /// <summary>
    /// Constructs the default material
    /// </summary>
    /// <param name="textures">Texture container for the scene</param>
    explicit Material(TextureContainer* textures);

    /// <summary>
    /// Constructs a material out of a single color (diffuse)
    /// </summary>
    /// <param name="name">Name of the material</param>
    /// <param name="scope">Scope of the material</param>
    /// <param name="textures">Texture container for the scene</param>
    /// <param name="color">Diffuse color of the material</param>
    /// <param name="internal">Whether the material is for internal use only</param>
    explicit Material(const std::string& name, const std::string& scope, TextureContainer* textures, const glm::vec3& color = glm::vec3(1.0f), bool internal = false);

    /// <summary>
    /// Constructs a material out of a config file and preloaded textures
    /// </summary>
    /// <param name="name">Name of the material</param>
    /// <param name="scope">Scope of the material</param>
    /// <param name="config">Config to build material from</param>
    /// <param name="map_kd">Diffuse texture</param>
    /// <param name="map_ka">Ambient texture</param>
    /// <param name="map_ks">Specular texture</param>
    /// <param name="map_bump">Normal/Bump texture</param>
    /// <param name="map_ns">Specular highlight/Height texture</param>
    /// <param name="map_d">Alpha texture</param>
    explicit Material(const std::string& name, const std::string& scope, Config* config, 
        Texture* map_kd, Texture* map_ka, Texture* map_ks, Texture* map_bump, Texture* map_ns, Texture* map_d);

    /// <summary>
    /// Constructs a material out of preloaded textures
    /// </summary>
    /// <param name="name">Name of the material</param>
    /// <param name="scope">Scope of the material</param>
    /// <param name="map_kd">Diffuse texture</param>
    /// <param name="map_ka">Ambient texture</param>
    /// <param name="map_ks">Specular texture</param>
    /// <param name="map_bump">Normal/Bump texture</param>
    /// <param name="map_ns">Specular highlight/Height texture</param>
    /// <param name="map_d">Alpha texture</param>
    /// <param name="ka">Alpha color</param>
    /// <param name="kd">Diffuse color</param>
    /// <param name="ks">Specular color</param>
    /// <param name="ns">Specular exponent</param>
    /// <param name="ni">Index of refraction</param>
    /// <param name="d">Alpha</param>
    /// <param name="ke">Emissive color</param>
    /// <param name="illum">Illumination mode</param>
    explicit Material(const std::string& name, const std::string& scope, 
        Texture* map_kd, Texture* map_ka, Texture* map_ks, Texture* map_bump, Texture* map_ns, Texture* map_d,
        const glm::vec3& ka = glm::vec3(), const glm::vec3& kd = glm::vec3(), const glm::vec3& ks = glm::vec3(),
        float ns = 0, float ni = 1, float d = 1, const glm::vec3& ke = glm::vec3(), int illum = 2);

    /// <summary>
    /// Constructs a material out of lists of preloaded textures
    /// </summary>
    /// <param name="name">Name of the material</param>
    /// <param name="scope">Scope of the material</param>
    /// <param name="map_kd">Diffuse textures</param>
    /// <param name="map_ka">Ambient textures</param>
    /// <param name="map_ks">Specular textures</param>
    /// <param name="map_bump">Normal/Bump textures</param>
    /// <param name="map_ns">Specular highlight/Height textures</param>
    /// <param name="map_d">Alpha textures</param>
    /// <param name="ka">Alpha color</param>
    /// <param name="kd">Diffuse color</param>
    /// <param name="ks">Specular color</param>
    /// <param name="ns">Specular exponent</param>
    /// <param name="ni">Index of refraction</param>
    /// <param name="d">Alpha</param>
    /// <param name="ke">Emissive color</param>
    /// <param name="illum">Illumination mode</param>
    explicit Material(const std::string& name, const std::string& scope,
        const std::vector<Texture*>& map_kd, const std::vector<Texture*>& map_ka, const std::vector<Texture*>& map_ks, 
        const std::vector<Texture*>& map_bump, const std::vector<Texture*>& map_ns, const std::vector<Texture*>& map_d,
        const glm::vec3& ka = glm::vec3(), const glm::vec3& kd = glm::vec3(), const glm::vec3& ks = glm::vec3(),
        float ns = 0, float ni = 1, float d = 1, const glm::vec3& ke = glm::vec3(), int illum = 2);

    /// <summary>
    /// Constructs a material out of unloaded texture names
    /// </summary>
    /// <param name="name">Name of the material</param>
    /// <param name="scope">Scope of the material</param>
    /// <param name="map_kd">Diffuse texture</param>
    /// <param name="map_ka">Ambient texture</param>
    /// <param name="map_ks">Specular texture</param>
    /// <param name="map_bump">Normal/Bump texture</param>
    /// <param name="map_ns">Specular highlight/Height texture</param>
    /// <param name="map_d">Alpha texture</param>
    /// <param name="ka">Alpha color</param>
    /// <param name="kd">Diffuse color</param>
    /// <param name="ks">Specular color</param>
    /// <param name="ns">Specular exponent</param>
    /// <param name="ni">Index of refraction</param>
    /// <param name="d">Alpha</param>
    /// <param name="ke">Emissive color</param>
    /// <param name="illum">Illumination mode</param>
    explicit Material(const std::string& name, const std::string& scope, const std::string& map_kd, const std::string& map_ka, const std::string& map_ks,
        const std::string& map_bump, const std::string& map_ns, const std::string& map_d,
        const glm::vec3& ka = glm::vec3(), const glm::vec3& kd = glm::vec3(), const glm::vec3& ks = glm::vec3(),
        float ns = 0, float ni = 1, float d = 1, const glm::vec3& ke = glm::vec3(), int illum = 2);
};