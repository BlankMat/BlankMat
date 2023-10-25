#pragma once
#include "interfaces/iEntity.h"
#include "primitives/pLine.h"
#include "primitives/pWireCube.h"

class PHandle : public IEntity
{
protected:
	PLine* mXHandle;
	PLine* mYHandle;
	PLine* mZHandle;
	PWireCube* mAllHandle;

	bool mXHeld = false;
	bool mYHeld = false;
	bool mZHeld = false;
	bool mAllHeld = false;

	void GenBuffers() override {}
public:

	void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats = false) override
	{
		// Don't draw any part of the object if not enabled
		if (!mIsEnabled)
			return;

		if (!(mYHeld || mZHeld || mAllHeld))
			mXHandle->Draw(shader, state, defaultMat, viewProj, drawMats);
		if (!(mXHeld || mZHeld || mAllHeld))
			mYHandle->Draw(shader, state, defaultMat, viewProj, drawMats);
		if (!(mXHeld || mYHeld || mAllHeld))
			mZHandle->Draw(shader, state, defaultMat, viewProj, drawMats);
		if (!(mXHeld || mYHeld || mZHeld))
			mAllHandle->Draw(shader, state, defaultMat, viewProj, drawMats);
	}

	// Sets the parent model matrices of all the components of this handle
	void SetParentModelMatrix(const glm::mat4& parentModelMatrix) override
	{
		mXHandle->SetParentModelMatrix(parentModelMatrix);
		mYHandle->SetParentModelMatrix(parentModelMatrix);
		mZHandle->SetParentModelMatrix(parentModelMatrix);
		mAllHandle->SetParentModelMatrix(parentModelMatrix);
	}

	void DrawShadows(Shader* shader, State* state) override {}

	PHandle(const std::string& name, const float len, const float lineWidth, const bool drawOver,
		const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f),
		const glm::vec3& allColor = glm::vec3(1, 1, 0),
		const glm::vec3& xColor = glm::vec3(1,0,0), const glm::vec3& yColor = glm::vec3(0,1,0), const glm::vec3& zColor = glm::vec3(0,0,1))
		: IEntity(name, nullptr, drawOver, pos, rot, scale)
	{
		mXHandle = new PLine(name + "x", glm::vec3(len * 0.05f, 0, 0), glm::vec3(len, 0, 0), new Material(xColor), lineWidth, drawOver);
		mYHandle = new PLine(name + "y", glm::vec3(0, len * 0.05f, 0), glm::vec3(0, len, 0), new Material(yColor), lineWidth, drawOver);
		mZHandle = new PLine(name + "z", glm::vec3(0, 0, len * 0.05f), glm::vec3(0, 0, len), new Material(zColor), lineWidth, drawOver);
		mAllHandle = new PWireCube(name + "all", len * 0.1f, new Material(allColor), lineWidth * 0.5f, drawOver);

		mXHeld = false;
		mYHeld = false;
		mZHeld = false;
		mAllHeld = false;
	}

	~PHandle()
	{
		delete mXHandle;
		delete mYHandle;
		delete mZHandle;
		delete mAllHandle;
	}
};