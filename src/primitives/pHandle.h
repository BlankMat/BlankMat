#pragma once
#include "rendering/iEntity.h"
#include "rendering/iPrimitive.h"
#include "pLine.h"
#include "pWireCube.h"

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

	void Draw(glm::mat4 viewProj, Camera* camera, Light* light, glm::mat4 model = glm::mat4(1.0f))
	{
		// Don't draw any part of the object if not enabled
		if (!mIsEnabled)
			return;

		glm::mat4 newModel = IEntity::GetModelMatrix() * model;
		if (!(mYHeld || mZHeld || mAllHeld))
			mXHandle->Draw(viewProj, camera, light, newModel);
		if (!(mXHeld || mZHeld || mAllHeld))
			mYHandle->Draw(viewProj, camera, light, newModel);
		if (!(mXHeld || mYHeld || mAllHeld))
			mZHandle->Draw(viewProj, camera, light, newModel);
		if (!(mXHeld || mYHeld || mZHeld))
			mAllHandle->Draw(viewProj, camera, light, newModel);
	}

	PHandle(float len, Shader* shader, float lineWidth, bool drawOver,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 allColor = glm::vec3(1, 1, 0),
		glm::vec3 xColor = glm::vec3(1,0,0), glm::vec3 yColor = glm::vec3(0,1,0), glm::vec3 zColor = glm::vec3(0,0,1))
		: IEntity(shader, nullptr, drawOver, pos, rot, scale)
	{
		mXHandle = new PLine(glm::vec3(len*0.05f,0,0), glm::vec3(len,0,0), shader, new Material(xColor), lineWidth, drawOver);
		mYHandle = new PLine(glm::vec3(0,len*0.05f,0), glm::vec3(0,len,0), shader, new Material(yColor), lineWidth, drawOver);
		mZHandle = new PLine(glm::vec3(0,0,len*0.05f), glm::vec3(0,0,len), shader, new Material(zColor), lineWidth, drawOver);
		mAllHandle = new PWireCube(len*0.1f, shader, new Material(allColor), lineWidth*0.5f, drawOver);

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