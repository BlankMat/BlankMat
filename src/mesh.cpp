#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    SetupMesh();
}

glm::vec3 Mesh::CalcCenter()
{
    glm::vec3 center = glm::vec3(0.0f);
    for (unsigned int i = 0; i < vertices.size(); i++)
        center += vertices[i].pos;
    return center;
}

void Mesh::CalcBasis()
{
    forward = glm::vec3(
        cos(rotation.x) * cos(rotation.y),
        sin(rotation.y),
        sin(rotation.x) * cos(rotation.y)
    );

    // Right vector
    right = glm::normalize(glm::cross(forward, worldUp));

    // Up vector : perpendicular to both direction and right
    up = glm::cross(right, forward);
}

glm::mat4 Mesh::GetModelMatrix()
{
    glm::mat4 identity = glm::mat4(1.0f);

    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(0, 1, 0));
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(1, 0, 0));
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));

    glm::mat4 rotate = rotateZ * rotateY * rotateX;
    return glm::scale(glm::translate(rotate, position), scale);
}

Vertex* Mesh::GetVertex(unsigned int index)
{
    if (index < vertices.size())
        return nullptr;
    return &vertices[index];
}

void Mesh::SetupMesh()
{
    position = CalcCenter();
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

// Render the mesh
void Mesh::Draw(glm::mat4 viewProj)
{
    // Bind all textures
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    unsigned int normalNum = 1;
    unsigned int heightNum = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNum++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNum++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        shader->SetInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Set uniforms for this draw
    glm::mat4 modelMatrix = GetModelMatrix();
    glm::mat4 mvp = viewProj * modelMatrix;
    glm::mat3 normalModel = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    shader->SetMat4("MVP", mvp);
    shader->SetMat4("Model", modelMatrix);
    shader->SetMat3("NormalModel", normalModel);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Reset active texture
    glActiveTexture(GL_TEXTURE0);
}