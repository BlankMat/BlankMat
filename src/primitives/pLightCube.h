#pragma once
#include "pCube.h"
#include "options.h"
#include "rendering/iLight.h"
#include "rendering/shader.h"

class PLightCube : public ILight
{
protected:
    PCube* mCube;
public:
    void Draw(glm::mat4 viewProj)
    {
        mCube->Draw(viewProj);
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
		float ka = 0.1f, float ks = 0.5f)
        : ILight(pos, dir, color, ka, ks)
    {
        mCube = new PCube(size, shader, color, 0.0f, false, pos);
    }

    PLightCube(float size, Shader* shader, Options* options)
        : PLightCube(size, shader, options->lightPos, options->lightDir, options->lightColor, options->lightKA, options->lightKS) {}

    void Cleanup() override
    {
        delete mCube;
        IEntity::Cleanup();
    }
};