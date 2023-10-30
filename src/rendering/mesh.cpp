#include "mesh.h"

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material* material)
    : IMesh(name, material)
{
    mVertices = vertices;
    mIndices = indices;

    Mesh::GenBuffers();
}

const glm::vec3 Mesh::CalcCenter()
{
    glm::vec3 center = glm::vec3(0.0f);
    for (unsigned int i = 0; i < mVertices.size(); i++)
        center += mVertices[i].pos;
    return center;
}

Mesh::~Mesh()
{
    mVertices.clear();
    mIndices.clear();
}