#include "material.h"
#include "tools/state.h"

// Updates the given shader with this material's properties
void Material::UpdateShader(Shader* _shader, State* _state, Material* _defaultMat)
{
    _shader->SetVec3("material.diffuse", kd);
    _shader->SetVec3("material.ambient", ka);
    _shader->SetVec3("material.specular", ks);
    _shader->SetVec3("material.emissive", ke);
    _shader->SetFloat("material.shininess", ns);
    _shader->SetFloat("material.refraction", ni);
    _shader->SetFloat("material.alpha", d);
    _shader->SetInt("material.mode", illum);

    // Read state (or lack of state)
    bool useDiffuse = (_state == nullptr || _state->enableDiffuseMap);
    bool useAmbient = (_state == nullptr || _state->enableAmbientMap);
    bool useSpecular = (_state == nullptr || _state->enableSpecularMap);
    bool useNormal = (_state == nullptr || _state->enableNormalMap);
    bool useHeight = (_state == nullptr || _state->enableHeightMap);
    bool useAlpha = (_state == nullptr || _state->enableAlphaMap);

    // Bind all textures
    unsigned int diffuseNum = 1;
    unsigned int ambientNum = 1;
    unsigned int specularNum = 1;
    unsigned int normalNum = 1;
    unsigned int heightNum = 1;
    unsigned int alphaNum = 1;
    for (unsigned int i = 0; i < mTextures.size(); i++)
    {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        // Retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = mTextures[i]->type;
        unsigned int loadID = 0;
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNum++);
            loadID = useDiffuse ? mTextures[i]->id : _defaultMat->map_kd->id;
            _shader->SetVec3("material.diffuse", useDiffuse ? glm::vec3(1.0f) : kd);
        }
        else if (name == "texture_ambient")
        {
            number = std::to_string(ambientNum++);
            loadID = useAmbient ? mTextures[i]->id : _defaultMat->map_ka->id;
            _shader->SetVec3("material.ambient", useAmbient ? glm::vec3(1.0f) : ka);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNum++);
            loadID = useSpecular ? mTextures[i]->id : _defaultMat->map_ks->id;
            _shader->SetVec3("material.specular", useSpecular ? glm::vec3(1.0f) : ks);
        }
        else if (name == "texture_normal")
        {
            number = std::to_string(normalNum++);
            loadID = useNormal ? mTextures[i]->id : _defaultMat->map_bump->id;
        }
        else if (name == "texture_height")
        {
            number = std::to_string(heightNum++);
            loadID = useHeight ? mTextures[i]->id : _defaultMat->map_ns->id;
        }
        else if (name == "texture_alpha")
        {
            number = std::to_string(alphaNum++);
            loadID = useAlpha ? mTextures[i]->id : _defaultMat->map_d->id;
        }

        // Set the sampler to the correct texture unit
        _shader->SetInt(("material." + name + number).c_str(), loadID);
        glBindTexture(GL_TEXTURE_2D, loadID);
    }
    glActiveTexture(GL_TEXTURE0);
}

// Constructs a material out of a single color (diffuse)
Material::Material(glm::vec3 _color, std::string _name)
{
    name = _name;
    kd = _color;
    ka = glm::vec3(0.0f);
    ks = glm::vec3(1.0f);
    ke = glm::vec3(0.0f);
    ns = 8.0f;
    ni = 0.0f;
    d = 1.0f;
    illum = 2;

    map_kd = nullptr;
    map_ka = nullptr;
    map_ks = nullptr;
    map_bump = nullptr;
    map_ns = nullptr;
    map_d = nullptr;
}

// Constructs a material out of a config file and preloaded textures
Material::Material(std::string _name, Config* _config, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
    Texture* _map_bump, Texture* _map_ns, Texture* _map_d)
{
    name = _name;

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
}

// Constructs a material out of preloaded textures
Material::Material(std::string _name, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
    Texture* _map_bump, Texture* _map_ns, Texture* _map_d,
    glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks,
    float _ns, float _ni, float _d, glm::vec3 _ke, int _illum)
    : IMaterial(_name, _ka, _kd, _ks, _ns, _ni, _d, _ke, _illum)
{
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
}

// Construcst a material out of lists of preloaded textures
Material::Material(std::string _name, std::vector<Texture*>& _map_kd, std::vector<Texture*>& _map_ka, std::vector<Texture*>& _map_ks,
    std::vector<Texture*>& _map_bump, std::vector<Texture*>& _map_ns, std::vector<Texture*>& _map_d,
    glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks,
    float _ns, float _ni, float _d, glm::vec3 _ke, int _illum)
    : IMaterial(_name, _ka, _kd, _ks, _ns, _ni, _d, _ke, _illum)
{
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
}