#pragma once
#include "pCube.h"
#include "rendering/light.h"
#include "rendering/shader.h"
#include "files/config.h"

class PLightCube : public Light
{
protected:
    PCube* mCube;
public:
    // Draws the object to the screen
    void Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model = glm::mat4(1.0f)) override
    {
        mCube->Draw(viewProj, camera, light, model);
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
    }

	PLightCube(std::string name, float size, Shader* shader, Material* defaultMat, State* state, glm::vec3 pos = glm::vec3(1.0f), 
        glm::vec3 dir = glm::vec3(-1.0f), glm::vec3 color = glm::vec3(1.0f), float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, bool gamma = false)
        : Light(LightType::POINT, pos, dir, color, kd, ka, ks, gamma)
    {
        mCube = new PCube(name, size, shader, new Material(color), defaultMat, state, 0.0f, false, pos);
    }

    PLightCube(std::string name, float size, Shader* shader, Material* defaultMat, State* state, Config* config)
        : PLightCube(name, size, shader, defaultMat, state, config->GetVec("pos"), config->GetVec("dir"), config->GetVec("color"), 
            config->GetFloat("diffuse"), config->GetFloat("ambient"), config->GetFloat("specular"), config->GetBool("gamma")) {}

    ~PLightCube()
    {
        delete mCube;
    }
};