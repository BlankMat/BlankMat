#pragma once
#include "glIncludes.h"
#include "iEntity.h"
#include "vertex.h"
#include <vector>

class IMesh : public IEntity
{
protected:
	glm::vec3 mFront;
	glm::vec3 mRight;
	glm::vec3 mUp;

	std::string mName;
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;

	// Generates the required buffers to render the mesh
	void GenBuffers() override
	{
		mVAO = mVBO = mEBO = 0;
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
public:
	// Sets the shader of the mesh
	virtual void SetShader(Shader* shader) = 0;

	// Returns the name of the mesh
	std::string GetName() { return mName; }
	
	// Sets the name of the mesh
	void SetName(std::string name) { mName = name; }

	// Returns the vertex with the given index
	Vertex* GetVertex(unsigned int index)
	{
		if (index < mVertices.size())
			return nullptr;
		return &mVertices[index];
	}

	// Returns the up vector of the mesh
	glm::vec3 GetUp() { return mUp; }

	// Returns the right vector of the mesh
	glm::vec3 GetRight() { return mRight; }

	// Returns the front vector of the mesh
	glm::vec3 GetFront() { return mFront; }

	// Adds delta to the position of the mesh
	void Translate(glm::vec3 delta) { mPos += delta; }

	// Adds delta to the rotation of the mesh
	void Rotate(glm::vec3 delta) { mRot += delta; }

	// Adds delta to the scale of the mesh
	void Scale(glm::vec3 delta) { mScale += delta; }

	// Calculates the basis (front/right/up vectors) of the mesh
	void CalcBasis()
	{
		mFront = glm::normalize(glm::vec3(
			cos(mRot.y) * sin(mRot.x),
			sin(mRot.y),
			cos(mRot.y) * cos(mRot.x)
		));

		// Right vector
		mRight = glm::normalize(glm::cross(mFront, glm::vec3(0.0f, 1.0f, 0.0f)));

		// Up vector : perpendicular to both direction and right
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}
};