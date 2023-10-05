#pragma once
#include "glIncludes.h"
#include "shader.h"

class IEntity {
protected:
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
	bool mDrawOver;

	glm::vec3 mPos;
	glm::vec3 mRot;
	glm::vec3 mScale;
	bool mIsEnabled;

	glm::vec3 mColor;

	Shader* mShader;

	// Generates the necessary VAO, VBO, and EBO for the object after verts and indices are set
	virtual void GenBuffers() = 0;
public:
	// Draws the object to the screen
	virtual void Draw(glm::mat4 viewProj) = 0;
	// Gets the position of the object
	virtual glm::vec3 GetPos() { return mPos; }
	// Gets the rotation of the object
	virtual glm::vec3 GetRot() { return mRot; }
	// Gets the scale of the object
	virtual glm::vec3 GetScale() { return mScale; }
	// Gets the color of the object
	virtual glm::vec3 GetColor() { return mColor; }
	// Sets the position of the object
	virtual void SetPos(glm::vec3 pos) { mPos = pos; }
	// Sets the rotation of the object
	virtual void SetRot(glm::vec3 rot) { mRot = rot; }
	// Sets the scale of the object
	virtual void SetScale(glm::vec3 scale) { mScale = scale; }
	// Sets the scale of the object
	virtual void SetColor(glm::vec3 color) { mColor = color; }

	// Returns whether the object is enabled
	bool IsEnabled() { return mIsEnabled; }
	// Enables the object
	void Enable() { mIsEnabled = true; }
	// Disables the object
	void Disable() { mIsEnabled = false; }
	// Toggles the enabled status of the object
	bool ToggleEnabled() { mIsEnabled = !mIsEnabled; return mIsEnabled; }

	// Returns the model matrix for the object
	glm::mat4 GetModelMatrix()
	{
		glm::mat4 identity = glm::mat4(1.0f);

		glm::mat4 rotateX = glm::rotate(identity, glm::radians(mRot.x), glm::vec3(1, 0, 0));
		glm::mat4 rotateY = glm::rotate(identity, glm::radians(mRot.y), glm::vec3(0, 1, 0));
		glm::mat4 rotateZ = glm::rotate(identity, glm::radians(mRot.z), glm::vec3(0, 0, 1));

		glm::mat4 translate = glm::translate(identity, mPos);
		glm::mat4 rotate = rotateY * rotateZ * rotateX;
		glm::mat4 scale = glm::scale(identity, mScale);
		return translate * rotate * scale;
	}

	IEntity(Shader* shader = nullptr, glm::vec3 color = glm::vec3(), bool drawOver = false,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: mShader(shader), mColor(color), mDrawOver(drawOver), mPos(pos), mRot(rot), mScale(scale)
	{
		mVAO = mVBO = mEBO = 0;
		mIsEnabled = true;
	}

	virtual void Cleanup()
	{
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mEBO);
	}
};