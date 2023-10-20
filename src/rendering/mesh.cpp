#include "mesh.h"

// Instantiates an empty mesh
Mesh::Mesh(std::string name, Material* material)
{
    mMaterial = material;
    mName = name;
}

Mesh::Mesh(std::string name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material)
{
    mVertices = vertices;
    mIndices = indices;
    mMaterial = material;
    mName = name;

    mPos = glm::vec3(0.0f);
    mRot = glm::vec3(0.0f);
    mScale = glm::vec3(1.0f);

    Mesh::GenBuffers();
}

// Render the mesh
void Mesh::Draw(Shader* shader, State* state, Material* defaultMat, glm::mat4 viewProj, glm::mat4 model)
{
    // Don't draw disabled meshes
    if (!mIsEnabled)
        return;

    // Set uniforms for this draw
    glm::mat4 modelMatrix = model * GetModelMatrix();
    glm::mat4 mvp = viewProj * modelMatrix;
    glm::mat3 normalModel = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

    shader->SetMat4("MVP", mvp);
    shader->SetMat4("Model", modelMatrix);
    shader->SetMat3("NormalModel", normalModel);
    
    // Bind shadow map
    unsigned int shadowIndex = mMaterial->UpdateShader(shader, state, defaultMat);
    glActiveTexture(GL_TEXTURE0 + shadowIndex);
    glBindTexture(GL_TEXTURE_2D, state->depthMap);
    glActiveTexture(GL_TEXTURE0);

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