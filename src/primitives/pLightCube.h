#pragma once
#include "pCube.h"
#include "rendering/iLight.h"
#include "rendering/shader.h"
#include "files/config.h"

class PLightCube : public ILight
{
protected:
    PCube* mCube;
public:
    void Draw(glm::mat4 viewProj, glm::mat4 model = glm::mat4(1.0f)) override
    {
        mCube->Draw(viewProj, model);
    }

    void SetPos(glm::vec3 pos) override
    {
        mPos = pos;
        mCube->SetPos(pos);
    }

    void SetColor(glm::vec3 color) override
    {
        mColor = color;
        mCube->SetColor(color);
    }

	PLightCube(float size, Shader* shader, glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f), glm::vec3 color = glm::vec3(1.0f),
		float ka = 0.1f, float ks = 0.5f, bool gamma = false)
        : ILight(pos, dir, color, ka, ks, gamma)
    {
        mCube = new PCube(size, shader, color, 0.0f, false, pos);
    }

    PLightCube(float size, Shader* shader, Config* config)
        : PLightCube(size, shader, config->GetVec("pos"), config->GetVec("dir"), config->GetVec("color"), config->GetFloat("ambient"), config->GetFloat("specular"), config->GetBool("gamma")) {}

    void Cleanup() override
    {
        delete mCube;
        IEntity::Cleanup();
    }
};