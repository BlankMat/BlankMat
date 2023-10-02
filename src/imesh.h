#pragma once
#include "glIncludes.h"
#include "ientity.h"
#include "vertex.h"
#include <vector>

class IMesh : public IEntity
{
protected:
	glm::vec3 mFront;
	glm::vec3 mRight;
	glm::vec3 mUp;
public:
	// Returns the vertex with the given index
	virtual Vertex* GetVertex(unsigned int index) = 0;

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

	// Sets the shader of the mesh
	void SetShader(Shader* shader) { mShader = shader; }
};