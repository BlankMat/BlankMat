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
        mCube->GetMaterial()->kd = color;
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

	PLightCube(const std::string& name, const float size, const LightType type = LightType::POINT, 
        const glm::vec3& pos = glm::vec3(1.0f), const glm::vec3& dir = glm::vec3(-1.0f),
        const glm::vec3& color = glm::vec3(1.0f), const float kd = 1.0f, const float ka = 0.1f, const float ks = 0.5f, 
        const bool gamma = true, const float range = 13.0f, const float spotInner = 25, const float spotOuter = 35)
        : Light(type, pos, dir, color, kd, ka, ks, gamma, range, spotInner, spotOuter)
    {
        Material* newMat = new Material(color);
        mCube = new PCube(name, size, newMat, 0.0f, false, pos);
        mDirLine = new PLine(name, glm::vec3(0, 0, 0), glm::vec3(0, 0, 2), new Material(glm::vec3(1,1,0)), 15.0f, false, pos);
        SetDir(dir);
    }

    PLightCube(const std::string& name, const float size, Config* config)
        : PLightCube(name, size, static_cast<LightType>(config->GetInt("type")), 
            config->GetVec("pos"), config->GetVec("dir"), config->GetVec("color"), 
            config->GetFloat("diffuse"), config->GetFloat("ambient"), config->GetFloat("specular"), config->GetBool("gamma"),
            config->GetFloat("range"), config->GetFloat("spotInnerRadius"), config->GetFloat("spotOuterRadius")) {}

    ~PLightCube()
    {
        delete mCube;
    }
};