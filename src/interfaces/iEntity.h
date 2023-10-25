#pragma once
#include "glIncludes.h"
#include "rendering/shader.h"
#include "rendering/material.h"
#include "tools/state.h"

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
	std::string mName;

	Material* mMaterial;

	bool mRecalcMatrices;
	glm::mat4 mParentModelMatrix;
	glm::mat4 mSelfModelMatrix;
	glm::mat4 mModelMatrix;
	glm::mat3 mNormalModelMatrix;

	// Recalculates all matrices of the entity
	void RecalcMatrices()
	{
		glm::mat4 identity = glm::mat4(1.0f);

		glm::mat4 rotateX = glm::rotate(identity, glm::radians(mRot.x), glm::vec3(1, 0, 0));
		glm::mat4 rotateY = glm::rotate(identity, glm::radians(mRot.y), glm::vec3(0, 1, 0));
		glm::mat4 rotateZ = glm::rotate(identity, glm::radians(mRot.z), glm::vec3(0, 0, 1));

		glm::mat4 translate = glm::translate(identity, mPos);
		glm::mat4 rotate = rotateY * rotateZ * rotateX;
		glm::mat4 scale = glm::scale(identity, mScale);

		mSelfModelMatrix = translate * rotate * scale;
		mModelMatrix = mParentModelMatrix * mSelfModelMatrix;
		mNormalModelMatrix = glm::mat3(glm::transpose(glm::inverse(mModelMatrix)));
		mRecalcMatrices = false;
	}

	// Generates the necessary VAO, VBO, and EBO for the object after verts and indices are set
	virtual void GenBuffers() = 0;
public:
	// Draws the object to the screen
	virtual void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj) = 0;
	// Draws the object's shadows to the screen
	virtual void DrawShadows(Shader* shader, State* state) = 0;
	// Gets the position of the object
	virtual const glm::vec3 GetPos() { return mPos; }
	// Gets the rotation of the object
	virtual const glm::vec3 GetRot() { return mRot; }
	// Gets the scale of the object
	virtual const glm::vec3 GetScale() { return mScale; }
	// Gets the material of the object
	virtual Material* GetMaterial() { return mMaterial; }
	// Returns the name of the object
	virtual const std::string GetName() { return mName; }

	// Sets the position of the object
	virtual void SetPos(const glm::vec3& pos)
	{
		if (mPos == pos)
			return;

		mPos = pos;
		mRecalcMatrices = true;
	}

	// Sets the rotation of the object
	virtual void SetRot(const glm::vec3& rot)
	{
		if (mRot == rot)
			return;

		mRot = rot;
		mRecalcMatrices = true;
	}

	// Sets the scale of the object
	virtual void SetScale(const glm::vec3& scale)
	{
		if (mScale == scale)
			return;

		mScale = scale;
		mRecalcMatrices = true;
	}

	// Sets the material of the object
	virtual void SetMaterial(Material* material) { mMaterial = material; }
	// Sets the name of the object
	void SetName(const std::string& name) { mName = name; }

	// Returns whether the object is enabled
	bool IsEnabled() { return mIsEnabled; }
	// Enables or disables the object
	void Enable(const bool shouldEnable = true) { mIsEnabled = shouldEnable; }
	// Toggles the enabled status of the object
	bool ToggleEnabled() { mIsEnabled = !mIsEnabled; return mIsEnabled; }

	// Sets the entity's parent model matrix
	virtual void SetParentModelMatrix(const glm::mat4& parentModelMatrix)
	{
		mParentModelMatrix = parentModelMatrix;
		RecalcMatrices();
	}

	// Returns the model matrix for the object
	const glm::mat4& GetModelMatrix()
	{
		if (mRecalcMatrices)
		{
			RecalcMatrices();
		}
		return mModelMatrix;
	}

	// Returns the combined model matrices of the parents of this object
	const glm::mat4& GetParentModelMatrix()
	{
		return mParentModelMatrix;
	}

	// Returns the object's own model matrix (without parent influence)
	const glm::mat4& GetSelfModelMatrix()
	{
		return mSelfModelMatrix;
	}

	// Returns the inverse model matrix of the object
	const glm::mat3& GetNormalModelMatrix()
	{
		if (mRecalcMatrices)
		{
			RecalcMatrices();
		}
		return mNormalModelMatrix;
	}

	IEntity(const std::string& name = "", Material* material = nullptr, const bool drawOver = false,
		const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
		: mName(name), mMaterial(material), mDrawOver(drawOver), mPos(pos), mRot(rot), mScale(scale)
	{
		mVAO = mVBO = mEBO = 0;
		mModelMatrix = glm::mat4(1.0f);
		mSelfModelMatrix = glm::mat4(1.0f);
		mParentModelMatrix = glm::mat4(1.0f);
		mNormalModelMatrix = glm::mat3(1.0f);
		mRecalcMatrices = true;
		mIsEnabled = true;
	}

	virtual void Cleanup()
	{
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mEBO);
	}

	// Returns the name of the entity, or "null" if nullptr
	static const std::string GetNameNullSafe(IEntity* entity) { return (entity != nullptr) ? entity->GetName() : "null"; }
};