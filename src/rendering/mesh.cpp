#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material)
{
    mVertices = vertices;
    mIndices = indices;
    mMaterial = material;

    mPos = glm::vec3(0.0f);
    mRot = glm::vec3(0.0f);
    mScale = glm::vec3(1.0f);

    Mesh::GenBuffers();
}

// Render the mesh
void Mesh::Draw(glm::mat4 viewProj, glm::mat4 model)
{
    // Don't draw disabled meshes
    if (!mIsEnabled)
        return;

    // Set uniforms for this draw
    glm::mat4 modelMatrix = GetModelMatrix() * model;
    glm::mat4 mvp = viewProj * modelMatrix;
    glm::mat3 normalModel = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    mMaterial->UpdateShader(mShader, mState, mDefaultMat);
    mShader->SetMat4("MVP", mvp);
    mShader->SetMat4("Model", modelMatrix);
    mShader->SetMat3("NormalModel", normalModel);

    // Draw mesh
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Reset active texture
    glActiveTexture(GL_TEXTURE0);
}

glm::vec3 Mesh::CalcCenter()
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