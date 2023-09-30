#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "primitive.h"
#include <vector>

class Line : public Primitive<glm::vec3, glm::uvec4>
{
public:
    Line(glm::vec3 start, glm::vec3 end, Shader* shader, glm::vec3 color, float lineWidth, bool drawOver = true,
        glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
        : Primitive(shader, color, lineWidth, drawOver, pos, rot, scale)
    {
        mVertices.push_back(start);
        mVertices.push_back(end);
        mIndices.push_back(glm::uvec4(1, 2, 2, 1));

        Primitive::GenBuffers();
    }
};