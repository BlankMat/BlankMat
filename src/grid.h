#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "drawable.h"
#include <vector>

class Grid : public Drawable {
private:
	unsigned int VAO;
	unsigned int VBO;
    unsigned int EBO;
    float width;
    glm::vec3 color;
    Shader* shader;
    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec4> indices;

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
        glDrawElements(GL_LINES, (GLuint)indices.size()*4, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
	}

    Grid(int gridSize, float unitSize, float _width, glm::vec3 _color, Shader* _shader)
        : color(_color), width(_width), shader(_shader)
    {
        // Generate x lines for grid
        for (int x = -gridSize; x <= gridSize; x++) {
            for (int z = -gridSize; z <= gridSize; z++) {
                vertices.push_back(unitSize * glm::vec3(x, 0, z));

                // Don't index verts with no verts next to them
                if (x == gridSize || z == gridSize)
                    continue;

                // Calculate indices
                int i = x + gridSize;
                int j = z + gridSize;
                unsigned int r1 = j * (2 * gridSize + 1) + i;
                unsigned int r2 = (j + 1) * (2 * gridSize + 1) + i;
                indices.push_back(glm::uvec4(r1, r1+1, r1+1, r2+1));
                indices.push_back(glm::uvec4(r2+1, r2, r2, r1));
            }
        }
        // Bind VAOs and VBOs
        VAO = VBO = EBO = 0;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (GLuint)vertices.size()*sizeof(*vertices.data()), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLuint)indices.size()*sizeof(*indices.data()), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Print verts and indices
        std::cout << "Printing grid:" << std::endl << "- Vertices:" << std::endl;
        for (unsigned int i = 0; i < vertices.size(); i++) {
            std::cout << "[" << i << "]: (" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ")" << std::endl;
        }
        std::cout << "- Indices:" << std::endl;
        for (unsigned int i = 0; i < indices.size(); i++) {
            std::cout << "[" << i << "]: (" << indices[i].x << ", " << indices[i].y << ", " << indices[i].z << ", " << indices[i].w << ")" << std::endl;
        }
    }

    ~Grid()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};