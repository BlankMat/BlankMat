#include "material.h"
#include "tools/state.h"

// Returns whether the material is for internal use only or not.
bool Material::IsInternal()
{
    return mIsInternal;
}

// Updates the given shader with this material's properties
unsigned int Material::UpdateShader(Shader* _shader)
{
    _shader->SetVec3("material.diffuse", mCurKD);
    _shader->SetVec3("material.ambient", mCurKA);
    _shader->SetVec3("material.specular", mCurKS);
    _shader->SetVec3("material.emissive", mCurKE);
    _shader->SetFloat("material.shininess", ns);
    _shader->SetFloat("material.refraction", ni);
    _shader->SetFloat("material.alpha", d);
    _shader->SetInt("material.mode", illum);
    _shader->SetBool("useShadows", mShadowsEnabled);

    for (unsigned int i = 0; i < mCurTextures.size(); i++)
    {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        _shader->SetInt(mCurTextureNames[i], i);
        glBindTexture(GL_TEXTURE_2D, mCurTextures[i]->id);
    }
    glActiveTexture(GL_TEXTURE0);

    return (unsigned int)mTextures.size();
}

// Loads the textures of this material into the OpenGL context
void Material::LoadShaderTextures(State* _state, Material* _defaultMat)
{
    // If the state has the map enabled, and the texture is not the default texture, use the texture
    bool useDiffuse = (map_kd != nullptr && map_kd->id != _defaultMat->map_kd->id) && (_state == nullptr || _state->enableDiffuseMap);
    bool useAmbient = (map_ka != nullptr && map_ka->id != _defaultMat->map_ka->id) && (_state == nullptr || _state->enableAmbientMap);
    bool useSpecular = (map_ks != nullptr && map_ks->id != _defaultMat->map_ks->id) && (_state == nullptr || _state->enableSpecularMap);
    bool useNormal = (map_bump != nullptr && map_bump->id != _defaultMat->map_bump->id) && (_state == nullptr || _state->enableNormalMap);
    bool useHeight = (map_ns != nullptr && map_ns->id != _defaultMat->map_ns->id) && (_state == nullptr || _state->enableHeightMap);
    bool useAlpha = (map_d != nullptr && map_d->id != _defaultMat->map_d->id) && (_state == nullptr || _state->enableAlphaMap);

    // Bind all textures
    unsigned int diffuseNum = 1;
    unsigned int ambientNum = 1;
    unsigned int specularNum = 1;
    unsigned int normalNum = 1;
    unsigned int heightNum = 1;
    unsigned int alphaNum = 1;

    mCurTextures.clear();
    mCurTextureNames.clear();
    for (unsigned int i = 0; i < mTextures.size(); i++)
    {
        // Retrieve texture number (the N in texture_diffuseN)
        std::string number;
        TextureType type = mTextures[i]->type;
        switch (mTextures[i]->type)
        {
        case TextureType::DIFFUSE:
            number = std::to_string(diffuseNum++);
            mCurTextures.push_back(useDiffuse ? mTextures[i] : _defaultMat->map_kd);
            mCurKD = (useDiffuse ? glm::vec3(1.0f) : kd);
            break;
        case TextureType::AMBIENT:
            number = std::to_string(ambientNum++);
            mCurTextures.push_back(useAmbient ? mTextures[i] : _defaultMat->map_ka);
            mCurKA = (useAmbient ? glm::vec3(1.0f) : ka);
            break;
        case TextureType::SPECULAR:
            number = std::to_string(specularNum++);
            mCurTextures.push_back(useSpecular ? mTextures[i] : _defaultMat->map_ks);
            mCurKS = (useSpecular ? glm::vec3(1.0f) : ks);
            break;
        case TextureType::NORMAL:
            number = std::to_string(normalNum++);
            mCurTextures.push_back(useNormal ? mTextures[i] : _defaultMat->map_bump);
            break;
        case TextureType::HEIGHT:
            number = std::to_string(heightNum++);
            mCurTextures.push_back(useHeight ? mTextures[i] : _defaultMat->map_ns);
            break;
        case TextureType::ALPHA:
            number = std::to_string(alphaNum++);
            mCurTextures.push_back(useAlpha ? mTextures[i] : _defaultMat->map_d);
            break;
        default:
            break;
        }

        // Set the sampler to the correct texture unit
        mCurTextureNames.push_back("material." + Texture::TextureToTypeString(type) + number);
    }

    mShadowsEnabled = (_state == nullptr || _state->enableShadows);
}

// Loads the textures of the material from the scene's texture list
void Material::LoadMaterialTextures(TextureContainer* _textures)
{
    // Create textures for material
    map_kd = _textures->GetItem(mTargetMapKD);
    map_ka = _textures->GetItem(mTargetMapKA);
    map_ks = _textures->GetItem(mTargetMapKS);
    map_bump = _textures->GetItem(mTargetMapBump);
    map_ns = _textures->GetItem(mTargetMapNS);
    map_d = _textures->GetItem(mTargetMapD);

    // Push textures to render list
    mTextures.push_back(map_kd);
    mTextures.push_back(map_ka);
    mTextures.push_back(map_ks);
    mTextures.push_back(map_bump);
    mTextures.push_back(map_ns);
    mTextures.push_back(map_d);
}

// Constructs the default material
Material::Material(TextureContainer* _textures)
{
    mSelectableType = SelectableType::MATERIAL;
    // Initialize to default material values
    name = "default";
    kd = glm::vec3(0.6f);
    ka = glm::vec3(0.0f);
    ks = glm::vec3(1.0f);
    ke = glm::vec3(0.0f);
    ns = 8.0f;
    ni = 0.0f;
    d = 1.0f;
    illum = 2;

    // Setup current draw call
    mCurKD = kd;
    mCurKA = ka;
    mCurKS = ks;
    mCurKE = ke;
    mIsInternal = true;
    mShadowsEnabled = true;

    // Load default textures
    mTargetMapKD = "default_diffuse";
    mTargetMapKA = "default_ambient";
    mTargetMapKS = "default_specular";
    mTargetMapBump = "default_normal";
    mTargetMapNS = "default_height";
    mTargetMapD = "default_alpha";
    LoadMaterialTextures(_textures);
}

// Constructs a material out of a single color (diffuse)
Material::Material(const std::string& _name, const glm::vec3& _color, TextureContainer* _textures, bool _internal)
    : mIsInternal(_internal)
{
    mSelectableType = SelectableType::MATERIAL;
    if (name == "")
        name = "Color" + Vec3ToHex(_color);
    name = _name;
    kd = _color;
    ka = glm::vec3(0.0f);
    ks = glm::vec3(1.0f);
    ke = glm::vec3(0.0f);
    ns = 8.0f;
    ni = 0.0f;
    d = 1.0f;
    illum = 2;

    // Setup current draw call
    mCurKD = kd;
    mCurKA = ka;
    mCurKS = ks;
    mCurKE = ke;
    mShadowsEnabled = true;

    // Load default textures
    mTargetMapKD = "default_diffuse";
    mTargetMapKA = "default_ambient";
    mTargetMapKS = "default_specular";
    mTargetMapBump = "default_normal";
    mTargetMapNS = "default_height";
    mTargetMapD = "default_alpha";
    LoadMaterialTextures(_textures);
}

// Constructs a material out of a config file and preloaded textures
Material::Material(const std::string& _name, Config* _config, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
    Texture* _map_bump, Texture* _map_ns, Texture* _map_d)
    : name(_name)
{
    mSelectableType = SelectableType::MATERIAL;
    // Load values from config
    kd = _config->GetVec("kd");
    ka = _config->GetVec("ka");
    ks = _config->GetVec("ks");
    ke = _config->GetVec("ke");
    ns = _config->GetFloat("ns");
    ni = _config->GetFloat("ni");
    d = _config->GetFloat("d");
    illum = _config->GetInt("illum");

    // Load textures from pointers
    map_kd = _map_kd;
    map_ka = _map_ka;
    map_ks = _map_ks;
    map_bump = _map_bump;
    map_ns = _map_ns;
    map_d = _map_d;

    // Push textures to list
    mTextures.push_back(map_kd);
    mTextures.push_back(map_ka);
    mTextures.push_back(map_ks);
    mTextures.push_back(map_bump);
    mTextures.push_back(map_ns);
    mTextures.push_back(map_d);

    mCurKD = kd;
    mCurKA = ka;
    mCurKS = ks;
    mCurKE = ke;
    mShadowsEnabled = true;
    mIsInternal = false;
}

// Constructs a material out of preloaded textures
Material::Material(const std::string& _name, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
    Texture* _map_bump, Texture* _map_ns, Texture* _map_d,
    const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks,
    float _ns, float _ni, float _d, const glm::vec3& _ke, int _illum)
    : name(_name), ka(_ka), kd(_kd), ks(_ks), ns(_ns), ni(_ni), d(_d), ke(_ke), illum(_illum)
{
    mSelectableType = SelectableType::MATERIAL;
    map_kd = _map_kd;
    map_ka = _map_ka;
    map_ks = _map_ks;
    map_bump = _map_bump;
    map_ns = _map_ns;
    map_d = _map_d;

    mTextures.push_back(map_kd);
    mTextures.push_back(map_ka);
    mTextures.push_back(map_ks);
    mTextures.push_back(map_bump);
    mTextures.push_back(map_ns);
    mTextures.push_back(map_d);

    mCurKD = kd;
    mCurKA = ka;
    mCurKS = ks;
    mCurKE = ke;
    mShadowsEnabled = true;
    mIsInternal = false;
}

// Construcst a material out of lists of preloaded textures
Material::Material(const std::string& _name, 
    const std::vector<Texture*>& _map_kd, const std::vector<Texture*>& _map_ka, const std::vector<Texture*>& _map_ks,
    const std::vector<Texture*>& _map_bump, const std::vector<Texture*>& _map_ns, const std::vector<Texture*>& _map_d,
    const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks,
    float _ns, float _ni, float _d, const glm::vec3& _ke, int _illum)
    : name(_name), ka(_ka), kd(_kd), ks(_ks), ns(_ns), ni(_ni), d(_d), ke(_ke), illum(_illum)
{
    mSelectableType = SelectableType::MATERIAL;
    map_kd = _map_kd[0];
    map_ka = _map_ka[0];
    map_ks = _map_ks[0];
    map_bump = _map_bump[0];
    map_ns = _map_ns[0];
    map_d = _map_d[0];

    mTextures.insert(mTextures.end(), _map_kd.begin(), _map_kd.end());
    mTextures.insert(mTextures.end(), _map_ka.begin(), _map_ka.end());
    mTextures.insert(mTextures.end(), _map_ks.begin(), _map_ks.end());
    mTextures.insert(mTextures.end(), _map_bump.begin(), _map_bump.end());
    mTextures.insert(mTextures.end(), _map_ns.begin(), _map_ns.end());
    mTextures.insert(mTextures.end(), _map_d.begin(), _map_d.end());

    mCurKD = kd;
    mCurKA = ka;
    mCurKS = ks;
    mCurKE = ke;
    mShadowsEnabled = true;
    mIsInternal = false;
}

Material::Material(const std::string& _name, const std::string& _map_kd, const std::string& _map_ka, const std::string& _map_ks,
    const std::string& _map_bump, const std::string& _map_ns, const std::string& _map_d,
    const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks,
    float _ns, float _ni, float _d, const glm::vec3& _ke, int _illum)
    : name(_name), ka(_ka), kd(_kd), ks(_ks), ns(_ns), ni(_ni), d(_d), ke(_ke), illum(_illum)
{
    mSelectableType = SelectableType::MATERIAL;
    mTargetMapKD = _map_kd;
    mTargetMapKA = _map_ka;
    mTargetMapKS = _map_ks;
    mTargetMapBump = _map_bump;
    mTargetMapNS = _map_ns;
    mTargetMapD = _map_d;

    mCurKD = kd;
    mCurKA = ka;
    mCurKS = ks;
    mCurKE = ke;
    mShadowsEnabled = true;
}