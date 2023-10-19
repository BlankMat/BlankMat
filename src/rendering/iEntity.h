#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "material.h"
#include "camera.h"
#include "light.h"
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

	Material* mDefaultMat;
	Material* mMaterial;
	Shader* mShader;
	State* mState;

	// Generates the necessary VAO, VBO, and EBO for the object after verts and indices are set
	virtual void GenBuffers() = 0;
public:
	// Draws the object to the screen
	virtual void Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model = glm::mat4(1.0f)) = 0;
	// Gets the position of the object
	virtual glm::vec3 GetPos() { return mPos; }
	// Gets the rotation of the object
	virtual glm::vec3 GetRot() { return mRot; }
	// Gets the scale of the object
	virtual glm::vec3 GetScale() { return mScale; }
	// Gets the material of the object
	virtual Material* GetMaterial() { return mMaterial; }
	// Gets the shader of the object
	virtual Shader* GetShader() { return mShader; }
	// Returns the name of the object
	std::string GetName() { return mName; }

	// Sets the position of the object
	virtual void SetPos(glm::vec3 pos) { mPos = pos; }
	// Sets the rotation of the object
	virtual void SetRot(glm::vec3 rot) { mRot = rot; }
	// Sets the scale of the object
	virtual void SetScale(glm::vec3 scale) { mScale = scale; }
	// Sets the material of the object
	virtual void SetMaterial(Material* material) { mMaterial = material; }
	// Sets the default material of the object
	virtual void SetDefaultMat(Material* defaultMat) { mDefaultMat = defaultMat; }
	// Stores the state of the application
	virtual void SetState(State* state) { mState = state; }
	// Sets the shader of the object
	virtual void SetShader(Shader* shader) { mShader = shader; }
	// Sets the name of the object
	void SetName(std::string name) { mName = name; }

	// Returns whether the object is enabled
	bool IsEnabled() { return mIsEnabled; }
	// Enables or disables the object
	void Enable(bool shouldEnable = true) { mIsEnabled = shouldEnable; }
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

	IEntity(std::string name = "", Shader* shader = nullptr, Material* material = nullptr, Material* defaultMat = nullptr, State* state = nullptr, bool drawOver = false,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: mName(name), mShader(shader), mMaterial(material), mDefaultMat(defaultMat), mState(state), mDrawOver(drawOver), mPos(pos), mRot(rot), mScale(scale)
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

	// Returns the name of the entity, or "null" if nullptr
	static std::string GetNameNullSafe(IEntity* entity) { return (entity != nullptr) ? entity->GetName() : "null"; }
};