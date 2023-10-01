#pragma once
#include "glIncludes.h"
#include "texture.h"
#include "vertex.h"
#include "shader.h"
#include "entity.h"
#include <vector>

class Mesh : public Entity
{
protected:
	glm::vec3 mFront;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::vec3 mWorldUp;

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;

	void GenBuffers() override;
	glm::vec3 CalcCenter();
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	void Draw(glm::mat4 viewProj);

	glm::vec3 GetUp() { return mUp; }
	glm::vec3 GetRight() { return mRight; }
	glm::vec3 GetFront() { return mFront; }
	Vertex* GetVertex(unsigned int index);

	void Translate(glm::vec3 delta) { mPos += delta; }
	void Rotate(glm::vec3 delta) { mRot += delta; }
	void Scale(glm::vec3 delta) { mScale += delta; }

	void CalcBasis();
	void SetWorldUp(glm::vec3 worldUp) { mWorldUp = worldUp; }
	void SetShader(Shader* shader) { mShader = shader; }
};