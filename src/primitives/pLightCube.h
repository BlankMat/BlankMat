#pragma once
#include "pCube.h"
#include "pLine.h"
#include "rendering/light.h"
#include "rendering/shader.h"
#include "files/config.h"

class PLightCube : public Light
{
protected:
    PCube* mCube;
    PLine* mDirLine;
public:
    // Draws the object to the screen
    void Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model = glm::mat4(1.0f)) override
    {
        mCube->Draw(viewProj, camera, light, model);
        mDirLine->Draw(viewProj, camera, light, model);
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

	PLightCube(std::string name, float size, Shader* shader, Material* defaultMat, State* state,
        LightType type = LightType::POINT, glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f),
        glm::vec3 color = glm::vec3(1.0f), float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, bool gamma = true,
        float range = 13.0f, float spotInner = 25, float spotOuter = 35)
        : Light(type, pos, dir, color, kd, ka, ks, gamma, range, spotInner, spotOuter)
    {
        Material* newMat = new Material(color);
        mCube = new PCube(name, size, shader, newMat, defaultMat, state, 0.0f, false, pos);
        mDirLine = new PLine(name, glm::vec3(0, 0, 0), glm::vec3(0, 0, 2), shader, new Material(glm::vec3(1,1,0)), defaultMat, state, 15.0f, false, pos);
        SetDir(dir);
    }

    PLightCube(std::string name, float size, Shader* shader, Material* defaultMat, State* state, Config* config)
        : PLightCube(name, size, shader, defaultMat, state, static_cast<LightType>(config->GetInt("type")), 
            config->GetVec("pos"), config->GetVec("dir"), config->GetVec("color"), 
            config->GetFloat("diffuse"), config->GetFloat("ambient"), config->GetFloat("specular"), config->GetBool("gamma"),
            config->GetFloat("range"), config->GetFloat("spotInnerRadius"), config->GetFloat("spotOuterRadius")) {}

    ~PLightCube()
    {
        delete mCube;
    }
};