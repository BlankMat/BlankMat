#pragma once
#include "rendering/mesh.h"

class VPlane : public Mesh
{
public:
	VPlane(float size, Shader* shader, Material* material, Material* defaultMat, State* state,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
        : Mesh(material, defaultMat, state)
	{
        float h = size * 0.5f;
        // Generate positions
        glm::vec3 pos1(-h, 0,  h);
        glm::vec3 pos2(-h, 0, -h);
        glm::vec3 pos3( h, 0, -h);
        glm::vec3 pos4( h, 0,  h);

        // Generate UV coords
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);

        // Normal
        glm::vec3 nm(0.0f, 1.0f, 0.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        // Generate verts of plane
        mVertices.push_back(Vertex(pos1, nm, uv1, tangent1));
        mVertices.push_back(Vertex(pos2, nm, uv2, tangent1));
        mVertices.push_back(Vertex(pos3, nm, uv3, tangent1));
        mVertices.push_back(Vertex(pos4, nm, uv4, tangent1));

        // Indices
        mIndices.push_back(0);
        mIndices.push_back(1);
        mIndices.push_back(2);
        mIndices.push_back(0);
        mIndices.push_back(2);
        mIndices.push_back(3);

        // Reverse
        mIndices.push_back(2);
        mIndices.push_back(1);
        mIndices.push_back(0);
        mIndices.push_back(3);
        mIndices.push_back(2);
        mIndices.push_back(0);

        mPos = pos;
        mRot = rot;
        mScale = scale;

        SetShader(shader);
        Mesh::GenBuffers();
	}
};