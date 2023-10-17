#include "mesh.h"

// Instantiates an empty mesh
Mesh::Mesh(std::string name, Material* material, Material* defaultMat, State* state)
{
    mMaterial = material;
    mDefaultMat = defaultMat;
    mState = state;
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
void Mesh::Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model)
{
    // Don't draw disabled meshes
    if (!mIsEnabled)
        return;

    // Set uniforms for this draw
    mShader->Use();
    glm::mat4 modelMatrix = model * GetModelMatrix();
    glm::mat4 mvp = viewProj * modelMatrix;
    glm::mat3 normalModel = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    light->UpdateShader(mShader);
    mMaterial->UpdateShader(mShader, mState, mDefaultMat);
    mShader->SetVec3("viewPos", camera->GetPos());
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