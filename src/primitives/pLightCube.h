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
    void Draw(Shader* shader, State* state, Material* defaultMat, glm::mat4 viewProj, glm::mat4 model = glm::mat4(1.0f))
    {
        mCube->Draw(shader, state, defaultMat, viewProj, model);
        mDirLine->Draw(shader, state, defaultMat, viewProj, model);
    }

    void SetColor(glm::vec3 color) override
    {
        Light::SetColor(color); 
        mCube->GetMaterial()->kd = color;
    }

    void SetPos(glm::vec3 pos) override 
    { 
        Light::SetPos(pos); 
        mCube->SetPos(pos); 
        mDirLine->SetPos(pos);
    }

    void SetDir(glm::vec3 dir) override
    {
        Light::SetDir(dir);
        mDirLine->SetRot(GetRotationDegrees(dir));
    }

	PLightCube(std::string name, float size, LightType type = LightType::POINT, 
        glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f),
        glm::vec3 color = glm::vec3(1.0f), float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, bool gamma = true,
        float range = 13.0f, float spotInner = 25, float spotOuter = 35)
        : Light(type, pos, dir, color, kd, ka, ks, gamma, range, spotInner, spotOuter)
    {
        Material* newMat = new Material(color);
        mCube = new PCube(name, size, newMat, 0.0f, false, pos);
        mDirLine = new PLine(name, glm::vec3(0, 0, 0), glm::vec3(0, 0, 2), new Material(glm::vec3(1,1,0)), 15.0f, false, pos);
        SetDir(dir);
    }

    PLightCube(std::string name, float size, Config* config)
        : PLightCube(name, size, static_cast<LightType>(config->GetInt("type")), 
            config->GetVec("pos"), config->GetVec("dir"), config->GetVec("color"), 
            config->GetFloat("diffuse"), config->GetFloat("ambient"), config->GetFloat("specular"), config->GetBool("gamma"),
            config->GetFloat("range"), config->GetFloat("spotInnerRadius"), config->GetFloat("spotOuterRadius")) {}

    ~PLightCube()
    {
        delete mCube;
    }
};