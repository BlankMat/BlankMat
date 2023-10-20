#pragma once
#include "interfaces/iPrimitive.h"

class PLine : public IPrimitive<glm::vec3, glm::uvec4>
{
public:
    void SetStartPos(glm::vec3 start)
    {
        if (mVertices.size() > 0)
            mVertices[0] = start;
    }

    void SetEndPos(glm::vec3 end)
    {
        if (mVertices.size() > 1)
            mVertices[1] = end;
    }

    PLine(std::string name, glm::vec3 start, glm::vec3 end, Material* material, float lineWidth, bool drawOver = true,
        glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
        : IPrimitive(name, material, lineWidth, drawOver, pos, rot, scale)
    {
        mVertices.push_back(start);
        mVertices.push_back(end);
        mIndices.push_back(glm::uvec4(1, 2, 2, 1));

        IPrimitive::GenBuffers();
    }
};