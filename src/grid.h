#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "primitive.h"
#include <vector>

class Grid : public Primitive<glm::vec3, glm::uvec2>
{
public:
    Grid(int gridSize, float unitSize, Shader* shader, glm::vec3 color, float lineWidth, 
        glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
        : Primitive(shader, color, lineWidth, pos, rot, scale)
    {
        // Generate x lines for grid
        for (int x = -gridSize; x <= gridSize; x++) {
            for (int z = -gridSize; z <= gridSize; z++) {
                mVertices.push_back(unitSize * glm::vec3(x, 0, z));

                // Calculate indices
                int i = x + gridSize;
                int j = z + gridSize;
                unsigned int r1 = j * (2 * gridSize + 1) + i;
                unsigned int r2 = (j + 1) * (2 * gridSize + 1) + i;
                if (x < gridSize)
                    mIndices.push_back(glm::uvec2(r1, r1+1));
                if (z < gridSize)
                    mIndices.push_back(glm::uvec2(r1, r2));
            }
        }
        Primitive::GenBuffers();
    }
};