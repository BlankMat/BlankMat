#include "material.h"
#include "interaction/state.h"

// Returns whether the material is for internal use only or not.
bool Material::IsInternal()
{
    return mIsInternal;
}

// Updates the given shader with this material's properties
unsigned int Material::UpdateShader(Shader* shader)
{
    shader->SetVec3("material.diffuse", mCurKD);
    shader->SetVec3("material.ambient", mCurKA);
    shader->SetVec3("material.specular", mCurKS);
    shader->SetVec3("material.emissive", mCurKE);
    shader->SetFloat("material.shininess", mNS);
    shader->SetFloat("material.refraction", mNI);
    shader->SetFloat("material.alpha", mD);
    shader->SetInt("material.mode", mIllum);
    shader->SetBool("useShadows", mShadowsEnabled);

    for (unsigned int i = 0; i < mCurTextures.size(); i++)
    {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        shader->SetInt(mCurTextureNames[i], i);
        glBindTexture(GL_TEXTURE_2D, mCurTextures[i]->mID);
    }
    glActiveTexture(GL_TEXTURE0);

    return (unsigned int)mTextures.size();
}

// Loads the textures of this material into the OpenGL context
void Material::LoadShaderTextures(State* state, Material* defaultMat)
{
    // If the state has the map enabled, and the texture is not the default texture, use the texture
    bool useDiffuse = (mMapKD != nullptr && mMapKD->mID != defaultMat->mMapKD->mID) && (state == nullptr || state->enableDiffuseMap);
    bool useAmbient = (mMapKA != nullptr && mMapKA->mID != defaultMat->mMapKA->mID) && (state == nullptr || state->enableAmbientMap);
    bool useSpecular = (mMapKS != nullptr && mMapKS->mID != defaultMat->mMapKS->mID) && (state == nullptr || state->enableSpecularMap);
    bool useNormal = (mMapBump != nullptr && mMapBump->mID != defaultMat->mMapBump->mID) && (state == nullptr || state->enableNormalMap);
    bool useHeight = (mMapNS != nullptr && mMapNS->mID != defaultMat->mMapNS->mID) && (state == nullptr || state->enableHeightMap);
    bool useAlpha = (mMapD != nullptr && mMapD->mID != defaultMat->mMapD->mID) && (state == nullptr || state->enableAlphaMap);

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
        TextureType type = mTextures[i]->mType;
        switch (mTextures[i]->mType)
        {
        case TextureType::DIFFUSE:
            number = std::to_string(diffuseNum++);
            mCurTextures.push_back(useDiffuse ? mTextures[i] : defaultMat->mMapKD);
            mCurKD = (useDiffuse ? glm::vec3(1.0f) : mKD);
            break;
        case TextureType::AMBIENT:
            number = std::to_string(ambientNum++);
            mCurTextures.push_back(useAmbient ? mTextures[i] : defaultMat->mMapKA);
            mCurKA = (useAmbient ? glm::vec3(1.0f) : mKA);
            break;
        case TextureType::SPECULAR:
            number = std::to_string(specularNum++);
            mCurTextures.push_back(useSpecular ? mTextures[i] : defaultMat->mMapKS);
            mCurKS = (useSpecular ? glm::vec3(1.0f) : mKS);
            break;
        case TextureType::NORMAL:
            number = std::to_string(normalNum++);
            mCurTextures.push_back(useNormal ? mTextures[i] : defaultMat->mMapBump);
            break;
        case TextureType::HEIGHT:
            number = std::to_string(heightNum++);
            mCurTextures.push_back(useHeight ? mTextures[i] : defaultMat->mMapNS);
            break;
        case TextureType::ALPHA:
            number = std::to_string(alphaNum++);
            mCurTextures.push_back(useAlpha ? mTextures[i] : defaultMat->mMapD);
            break;
        default:
            break;
        }

        // Set the sampler to the correct texture unit
        mCurTextureNames.push_back("material." + Texture::TextureToTypeString(type) + number);
    }

    mShadowsEnabled = (state == nullptr || state->enableShadows);
}

// Loads the textures of the material from the scene's texture list
void Material::LoadMaterialTextures(TextureContainer* textures)
{
    // Create textures for material
    mMapKD = textures->GetItem(mTargetMapKD);
    mMapKA = textures->GetItem(mTargetMapKA);
    mMapKS = textures->GetItem(mTargetMapKS);
    mMapBump = textures->GetItem(mTargetMapBump);
    mMapNS = textures->GetItem(mTargetMapNS);
    mMapD = textures->GetItem(mTargetMapD);

    // Push textures to render list
    mTextures.push_back(mMapKD);
    mTextures.push_back(mMapKA);
    mTextures.push_back(mMapKS);
    mTextures.push_back(mMapBump);
    mTextures.push_back(mMapNS);
    mTextures.push_back(mMapD);
}

// Constructs the default material
Material::Material(TextureContainer* textures)
    : ISelectable(SelectableType::MATERIAL)
{
    // Initialize to default material values
    InitName("default", "");
    mKD = glm::vec3(0.6f);
    mKA = glm::vec3(0.0f);
    mKS = glm::vec3(1.0f);
    mKE = glm::vec3(0.0f);
    mNS = 8.0f;
    mNI = 0.0f;
    mD = 1.0f;
    mIllum = 2;

    // Setup current draw call
    mCurKD = mKD;
    mCurKA = mKA;
    mCurKS = mKS;
    mCurKE = mKE;
    mIsInternal = true;
    mShadowsEnabled = true;

    // Load default textures
    mTargetMapKD = "default_diffuse";
    mTargetMapKA = "default_ambient";
    mTargetMapKS = "default_specular";
    mTargetMapBump = "default_normal";
    mTargetMapNS = "default_height";
    mTargetMapD = "default_alpha";
    LoadMaterialTextures(textures);
}

// Constructs a material out of a single color (diffuse)
Material::Material(const std::string& name, const std::string& scope, TextureContainer* textures, const glm::vec3& color, bool internal)
    : mIsInternal(internal), ISelectable(SelectableType::MATERIAL)
{
    InitName((name != "" ? name : "Color" + Vec3ToHex(color)), scope);
    mKD = color;
    mKA = glm::vec3(0.0f);
    mKS = glm::vec3(1.0f);
    mKE = glm::vec3(0.0f);
    mNS = 8.0f;
    mNI = 0.0f;
    mD = 1.0f;
    mIllum = 2;

    // Setup current draw call
    mCurKD = mKD;
    mCurKA = mKA;
    mCurKS = mKS;
    mCurKE = mKE;
    mShadowsEnabled = true;

    // Load default textures
    mTargetMapKD = "default_diffuse";
    mTargetMapKA = "default_ambient";
    mTargetMapKS = "default_specular";
    mTargetMapBump = "default_normal";
    mTargetMapNS = "default_height";
    mTargetMapD = "default_alpha";
    LoadMaterialTextures(textures);
}

// Constructs a material out of a config file and preloaded textures
Material::Material(const std::string& name, const std::string& scope, Config* config, 
    Texture* map_kd, Texture* map_ka, Texture* map_ks, Texture* map_bump, Texture* map_ns, Texture* map_d)
    : ISelectable(SelectableType::MATERIAL)
{
    InitName(name, scope);
    // Load values from config
    mKD = config->GetVec("kd");
    mKA = config->GetVec("ka");
    mKS = config->GetVec("ks");
    mKE = config->GetVec("ke");
    mNS = config->GetFloat("ns");
    mNI = config->GetFloat("ni");
    mD = config->GetFloat("d");
    mIllum = config->GetInt("illum");

    // Load textures from pointers
    mMapKD = map_kd;
    mMapKA = map_ka;
    mMapKS = map_ks;
    mMapBump = map_bump;
    mMapNS = map_ns;
    mMapD = map_d;

    // Push textures to list
    mTextures.push_back(mMapKD);
    mTextures.push_back(mMapKA);
    mTextures.push_back(mMapKS);
    mTextures.push_back(mMapBump);
    mTextures.push_back(mMapNS);
    mTextures.push_back(mMapD);

    mCurKD = mKD;
    mCurKA = mKA;
    mCurKS = mKS;
    mCurKE = mKE;
    mShadowsEnabled = true;
    mIsInternal = false;
}

// Constructs a material out of preloaded textures
Material::Material(const std::string& name, const std::string& scope,
    Texture* map_kd, Texture* map_ka, Texture* map_ks, Texture* map_bump, Texture* map_ns, Texture* map_d,
    const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks, float ns, float ni, float d, const glm::vec3& ke, int illum)
    : mKA(ka), mKD(kd), mKS(ks), mNS(ns), mNI(ni), mD(d), mKE(ke), mIllum(illum), ISelectable(SelectableType::MATERIAL)
{
    InitName(name, scope);
    mMapKD = map_kd;
    mMapKA = map_ka;
    mMapKS = map_ks;
    mMapBump = map_bump;
    mMapNS = map_ns;
    mMapD = map_d;

    mTextures.push_back(mMapKD);
    mTextures.push_back(mMapKA);
    mTextures.push_back(mMapKS);
    mTextures.push_back(mMapBump);
    mTextures.push_back(mMapNS);
    mTextures.push_back(mMapD);

    mCurKD = mKD;
    mCurKA = mKA;
    mCurKS = mKS;
    mCurKE = mKE;
    mShadowsEnabled = true;
    mIsInternal = false;
}

// Construcst a material out of lists of preloaded textures
Material::Material(const std::string& name, const std::string& scope,
    const std::vector<Texture*>& map_kd, const std::vector<Texture*>& map_ka, const std::vector<Texture*>& map_ks,
    const std::vector<Texture*>& map_bump, const std::vector<Texture*>& map_ns, const std::vector<Texture*>& map_d,
    const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks,
    float ns, float ni, float d, const glm::vec3& ke, int illum)
    : mKA(ka), mKD(kd), mKS(ks), mNS(ns), mNI(ni), mD(d), mKE(ke), mIllum(illum), ISelectable(SelectableType::MATERIAL)
{
    InitName(name, scope);
    mMapKD = map_kd[0];
    mMapKA = map_ka[0];
    mMapKS = map_ks[0];
    mMapBump = map_bump[0];
    mMapNS = map_ns[0];
    mMapD = map_d[0];

    mTextures.insert(mTextures.end(), map_kd.begin(), map_kd.end());
    mTextures.insert(mTextures.end(), map_ka.begin(), map_ka.end());
    mTextures.insert(mTextures.end(), map_ks.begin(), map_ks.end());
    mTextures.insert(mTextures.end(), map_bump.begin(), map_bump.end());
    mTextures.insert(mTextures.end(), map_ns.begin(), map_ns.end());
    mTextures.insert(mTextures.end(), map_d.begin(), map_d.end());

    mCurKD = mKD;
    mCurKA = mKA;
    mCurKS = mKS;
    mCurKE = mKE;
    mShadowsEnabled = true;
    mIsInternal = false;
}

Material::Material(const std::string& name, const std::string& scope, const std::string& map_kd, const std::string& map_ka, const std::string& map_ks,
    const std::string& map_bump, const std::string& map_ns, const std::string& map_d,
    const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks,
    float ns, float ni, float d, const glm::vec3& ke, int illum)
    : mKA(ka), mKD(kd), mKS(ks), mNS(ns), mNI(ni), mD(d), mKE(ke), mIllum(illum), ISelectable(SelectableType::MATERIAL)
{
    InitName(name, scope);
    mTargetMapKD = map_kd;
    mTargetMapKA = map_ka;
    mTargetMapKS = map_ks;
    mTargetMapBump = map_bump;
    mTargetMapNS = map_ns;
    mTargetMapD = map_d;

    mCurKD = mKD;
    mCurKA = mKA;
    mCurKS = mKS;
    mCurKE = mKE;
    mShadowsEnabled = true;
}