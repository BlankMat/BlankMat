#pragma once
#include "glIncludes.h"
#include "shader.h"

class Drawable {
protected:
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;

	glm::vec3 mPos;
	glm::vec3 mRot;
	glm::vec3 mScale;

	glm::vec3 mColor;

	Shader* mShader;

	// Generates the necessary VAO, VBO, and EBO for the object after verts and indices are set
	virtual void GenBuffers() = 0;
public:
	// Draws the object to the screen
	virtual void Draw(glm::mat4 viewProj) = 0;

	// Returns the model matrix for the object
	glm::mat4 GetModelMatrix()
	{
		glm::mat4 identity = glm::mat4(1.0f);
		glm::mat4 rotateX = glm::rotate(identity, glm::radians(mRot.x), glm::vec3(0, 1, 0));
		glm::mat4 rotateY = glm::rotate(identity, glm::radians(mRot.y), glm::vec3(1, 0, 0));
		glm::mat4 rotateZ = glm::rotate(identity, glm::radians(mRot.z), glm::vec3(0, 0, 1));

		glm::mat4 rotate = rotateZ * rotateY * rotateX;
		return glm::scale(glm::translate(rotate, mPos), mScale);
	}

	Drawable(Shader* shader = nullptr, glm::vec3 color = glm::vec3(), 
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: mShader(shader), mColor(color), mPos(pos), mRot(rot), mScale(scale)
	{
		mVAO = mVBO = mEBO = 0;
	}

	virtual void Cleanup()
	{
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mEBO);
	}
};