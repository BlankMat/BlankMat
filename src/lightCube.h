#pragma once
#include "light.h"
#include "shader.h"
#include "cube.h"

class LightCube : public Light
{
protected:
    Cube* mCube;
public:
    void Draw(glm::mat4 viewProj)
    {
        mCube->Draw(viewProj);
    }

	LightCube(float size, Shader* shader, glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f), glm::vec3 color = glm::vec3(1.0f),
		float ka = 0.1f, float ks = 0.5f)
        : Light(pos, dir, color, ka, ks)
    {
        mCube = new Cube(size, shader, color, 0.0f, pos);
    }

    ~LightCube()
    {
        delete mCube;
    }
};