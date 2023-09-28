#pragma once
#include "light.h"
#include "shader.h"

struct LightCube : public Light
{
    float vertices[108] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };
    unsigned int lightCubeVAO;
    unsigned int lightCubeVBO;

	Shader* lightShader;

    void Draw(glm::mat4 viewProj)
    {
        if (lightShader != nullptr)
        {
            // Setup shader for use
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

            lightShader->Use();
            lightShader->SetMat4("MVP", viewProj * model);
            lightShader->SetVec3("LightColor", color);

            // Render the light cube
            glBindVertexArray(lightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void LoadShader(std::string name)
    {
        if (name == "")
            return;
        lightShader = new Shader(name);

        // Create VBO for shader
        unsigned int VBO;
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        lightCubeVAO = VBO;

        // Create VAO for shader
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        lightCubeVAO = VAO;

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        std::cout << "Loaded shader " << name << " for light cube" << std::endl;
    }

	LightCube(std::string _shaderName = "", glm::vec3 _pos = glm::vec3(1.0f), glm::vec3 _dir = glm::vec3(-1.0f), glm::vec3 _color = glm::vec3(1.0f),
		float _ka = 0.1f, float _ks = 0.5f) : Light(_pos, _dir, _color, _ka, _ks)
	{
        lightCubeVAO = 0;
        lightCubeVBO = 0;
		lightShader = nullptr;
        LoadShader(_shaderName);
	}

    LightCube::~LightCube()
    {
        if (lightShader != nullptr)
            delete lightShader;
    }
};