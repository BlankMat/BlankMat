#include "mesh.h"

// Generates a mesh from the given vertices, indices, material, and children
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
void Mesh::Draw(glm::mat4 viewProj)
{
    // Don't draw disabled meshes
    if (!mIsEnabled)
        return;

    // Set uniforms for this draw
    mShader->Use();
    glm::mat4 modelMatrix = GetModelMatrix();
    glm::mat4 mvp = viewProj * modelMatrix;
    glm::mat3 normalModel = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    mShader->SetMat4("MVP", mvp);
    mShader->SetMat4("Model", modelMatrix);
    mShader->SetMat3("NormalModel", normalModel);
    mMaterial->UpdateShader(mShader);

    // Draw mesh
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Recursively draw child meshes
    for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
        if (iter->second != nullptr)
            iter->second->Draw(viewProj);
}

// Adds the child mesh
IMesh* Mesh::AddChild(IMesh* child)
{
    std::string name = child->GetName();
    // If a duplicate name is found, change the name
    if (mChildren.find(name) == mChildren.end())
    {
        // If the name ends in a number, increment it
        size_t numIndex = name.find_last_not_of("0123456789");
        std::string nums = name.substr(numIndex + 1);
        // If the name does not end in a number, add _0 to the end
        name = (nums.size() > 0) ? name.substr(0, numIndex) + std::to_string(std::stoi(nums) + 1) : name + "_0";
        child->SetName(name);
    }
    mChildren.emplace(name, child);
    return child;
}

// Sets the shader for the mesh and its children
void Mesh::SetShader(Shader* shader)
{
    mShader = shader;
    for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
        if (iter->second != nullptr)
            iter->second->SetShader(shader);
}

// Cleans up the mesh
Mesh::~Mesh()
{
    mVertices.clear();
    mIndices.clear();
    for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
        if (iter->second != nullptr)
            delete iter->second;
    mChildren.clear();
}