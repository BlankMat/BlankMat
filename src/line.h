#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "drawable.h"
#include <vector>

class Line : public Drawable {
private:
    unsigned int VAO;
    unsigned int VBO;
    float width;
    glm::vec3 color;
    Shader* shader;
    std::vector<float> vertices;

public:
    void Draw(glm::mat4 viewProj)
    {
        // Setup shader for use
        shader->Use();
        shader->SetMat4("MVP", viewProj);
        shader->SetVec3("Color", color);
        shader->SetFloat("Width", width);

        // Render the light cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 36);
        glBindVertexArray(0);
    }

    Line(glm::vec3 _start, glm::vec3 _end, glm::vec3 _color, float _width, Shader* _shader) 
        : color(_color), width(_width), shader(_shader)
    {
        vertices.push_back(_start.x);
        vertices.push_back(_start.y);
        vertices.push_back(_start.z);
        vertices.push_back(_end.x);
        vertices.push_back(_end.y);
        vertices.push_back(_end.z);

        VAO = 0;
        VBO = 0;
        // Bind VAOs and VBOs
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Line()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};