#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
    mVertices = vertices;
    mIndices = indices;
    mTextures = textures;

    mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mPos = CalcCenter();
    mRot = glm::vec3(0.0f);
    mScale = glm::vec3(1.0f);

    GenBuffers();
}

// Render the mesh
void Mesh::Draw(glm::mat4 viewProj)
{
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

glm::vec3 Mesh::CalcCenter()
{
    glm::vec3 center = glm::vec3(0.0f);
    for (unsigned int i = 0; i < mVertices.size(); i++)
        center += mVertices[i].pos;
    return center;
}

void Mesh::CalcBasis()
{
    mFront = glm::vec3(
        cos(mRot.x) * cos(mRot.y),
        sin(mRot.y),
        sin(mRot.x) * cos(mRot.y)
    );

    // Right vector
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));

    // Up vector : perpendicular to both direction and right
    mUp = glm::cross(mRight, mFront);
}

Vertex* Mesh::GetVertex(unsigned int index)
{
    if (index < mVertices.size())
        return nullptr;
    return &mVertices[index];
}

void Mesh::GenBuffers()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, (GLsizei)mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}