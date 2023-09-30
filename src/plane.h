#pragma once
#include "primitive.h"

class Plane : public Primitive<glm::vec3, glm::uvec3>
{
public:
    Plane(float size, bool doubleSided, Shader* shader, glm::vec3 color, bool drawOver,
        glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
        : Primitive(shader, color, 0.0f, drawOver, pos, rot, scale)
    {
        // Generate verts of plane
        float h = size * 0.5f;
        mVertices.push_back(glm::vec3(-h, 0,  h));
        mVertices.push_back(glm::vec3( h, 0,  h));
        mVertices.push_back(glm::vec3(-h, 0, -h));
        mVertices.push_back(glm::vec3( h, 0, -h));
        mIndices.push_back(glm::uvec3(3, 2, 0));
        mIndices.push_back(glm::uvec3(0, 1, 3));

        // Draw flipped sides as well
        if (doubleSided) {
            mIndices.push_back(glm::uvec3(0, 2, 3));
            mIndices.push_back(glm::uvec3(3, 1, 0));
        }

        mIsWireframe = false;
        Primitive::GenBuffers();
    }
};