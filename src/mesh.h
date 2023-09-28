#pragma once
#include "glIncludes.h"
#include "texture.h"
#include "vertex.h"
#include "shader.h"
#include <vector>

class Mesh
{
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	void SetupMesh();
	glm::vec3 CalcCenter();
public:
	glm::vec3 GetPos() { return position; }
	glm::vec3 GetRotation() { return rotation; }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetUp() { return up; }
	glm::vec3 GetRight() { return right; }
	glm::vec3 GetForward() { return forward; }
	glm::mat4 GetModelMatrix();
	Vertex* GetVertex(unsigned int index);

	void SetPos(glm::vec3 _pos) { position = _pos; }
	void SetRotation(glm::vec3 _rot) { rotation = _rot; }
	void SetScale(glm::vec3 _scale) { scale = _scale; }

	void Translate(glm::vec3 delta) { position += delta; }
	void Rotate(glm::vec3 delta) { rotation += delta; }
	void Scale(glm::vec3 delta) { scale += delta; }

	void CalcBasis();
	void SetWorldUp(glm::vec3 _worldUp) { worldUp = _worldUp; }

	Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	void Draw(Shader& shader, glm::mat4 viewProj);
};