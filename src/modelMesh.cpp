#include "modelMesh.h"

ModelMesh::ModelMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
    mVertices = vertices;
    mIndices = indices;
    mTextures = textures;

    mPos = glm::vec3(0.0f);
    mRot = glm::vec3(0.0f);
    mScale = glm::vec3(1.0f);

    ModelMesh::GenBuffers();
}

// Render the mesh
void ModelMesh::Draw(glm::mat4 viewProj)
{
    // Don't draw disabled meshes
    if (!mIsEnabled)
        return;

    // Bind all textures
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    unsigned int normalNum = 1;
    unsigned int heightNum = 1;
    for (unsigned int i = 0; i < mTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = mTextures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNum++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNum++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        mShader->SetInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
    }

    // Set uniforms for this draw
    glm::mat4 modelMatrix = GetModelMatrix();
    glm::mat4 mvp = viewProj * modelMatrix;
    glm::mat3 normalModel = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
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

glm::vec3 ModelMesh::CalcCenter()
{
    glm::vec3 center = glm::vec3(0.0f);
    for (unsigned int i = 0; i < mVertices.size(); i++)
        center += mVertices[i].pos;
    return center;
}

ModelMesh::~ModelMesh()
{
    mVertices.clear();
    mIndices.clear();
    mTextures.clear();
}