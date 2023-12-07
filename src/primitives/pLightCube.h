#pragma once
#include "files/config.h"
#include "rendering/light.h"
#include "primitives/pLine.h"
#include "primitives/pCube.h"

class PLightCube : public Light
{
protected:
    PCube* mCube;
    PLine* mDirLine;
public:
    // Draws the object to the screen
    void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats = false)
    {
        mCube->Draw(shader, state, defaultMat, viewProj, drawMats);
        mDirLine->Draw(shader, state, defaultMat, viewProj, drawMats);
    }

    // Sets the parent model matrices of the children
    void SetParentModelMatrix(const glm::mat4& parentModelMatrix)
    {
        mCube->SetParentModelMatrix(parentModelMatrix);
        mDirLine->SetParentModelMatrix(parentModelMatrix);
    }

    void SetColor(const glm::vec3& color) override
    {
        Light::SetColor(color); 
        mCube->GetMaterial()->mKD = color;
    }

    void SetPos(const glm::vec3& pos) override 
    { 
        Light::SetPos(pos); 
        mCube->SetPos(pos); 
        mDirLine->SetPos(pos);
    }

    void SetDir(const glm::vec3& dir) override
    {
        Light::SetDir(dir);
        mDirLine->SetRot(GetRotationDegrees(dir));
    }

    void LoadMaterials(MaterialContainer* materials, TextureContainer* textures) override
    {
        mCube->SetMaterial(materials->AddMaterial(new Material("internal_lightMat", "", textures, mColor, true), true));
        mDirLine->SetMaterial(materials->AddMaterial(new Material("internal_yellow", "", textures, glm::vec3(1, 1, 0), true), true));
    }

    PLightCube(const std::string& name, const std::string& scope, LightType type = LightType::POINT, const glm::vec3& pos = glm::vec3(1.0f), const glm::vec3& dir = glm::vec3(-1.0f),
        const glm::vec3& color = glm::vec3(1.0f), float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, bool gamma = true,
        float range = 13.0f, float spotInner = 25, float spotOuter = 35)
        : Light(name, scope, type, pos, dir, color, kd, ka, ks, gamma, range, spotInner, spotOuter)
    {
        mIsCube = true;
        mCube = new PCube(name, scope, 1.0f, nullptr, 0.0f, false, pos);
        mDirLine = new PLine(name, scope, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1.5f), nullptr, 15.0f, false, pos);
        SetDir(dir);
    }

	PLightCube(float size, const std::string& name, const std::string& scope = "", LightType type = LightType::POINT,
        MaterialContainer* materials = nullptr, TextureContainer* textures = nullptr,
        const glm::vec3& pos = glm::vec3(-5.0f, 5.0f, 5.0f), const glm::vec3& dir = glm::vec3(5.0f, 5.0f, 5.0f),
        const glm::vec3& color = glm::vec3(1.0f), float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, 
        bool gamma = true, float range = 13.0f, float spotInner = 25, float spotOuter = 35)
        : Light(name, scope, type, pos, dir, color, kd, ka, ks, gamma, range, spotInner, spotOuter)
    {
        mIsCube = true;
        mCube = new PCube(name, scope, size, nullptr, 0.0f, false, pos);
        mDirLine = new PLine(name, scope, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1.5f), nullptr, 15.0f, false, pos);
        LoadMaterials(materials, textures);
        SetDir(dir);
    }

    PLightCube(float size, MaterialContainer* materials, TextureContainer* textures, Config* config)
        : PLightCube(size, config->GetString("name"), "", static_cast<LightType>(config->GetInt("type")), materials, textures,
            config->GetVec("pos"), config->GetVec("dir"), config->GetVec("color"), 
            config->GetFloat("diffuse"), config->GetFloat("ambient"), config->GetFloat("specular"), config->GetBool("gamma"),
            config->GetFloat("range"), config->GetFloat("spotInnerRadius"), config->GetFloat("spotOuterRadius"))
    {}

    ~PLightCube()
    {
        delete mCube;
        delete mDirLine;
    }
};