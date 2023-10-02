#pragma once
#include "iEntity.h"
#include "iPrimitive.h"
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

	void Draw(glm::mat4 viewProj)
	{
		// Don't draw any part of the object if not enabled
		if (!mIsEnabled)
			return;

		glm::mat4 MVP = viewProj * IEntity::GetModelMatrix();
		if (!(mYHeld || mZHeld || mAllHeld))
			mXHandle->Draw(MVP);
		if (!(mXHeld || mZHeld || mAllHeld))
			mYHandle->Draw(MVP);
		if (!(mXHeld || mYHeld || mAllHeld))
			mZHandle->Draw(MVP);
		if (!(mXHeld || mYHeld || mZHeld))
			mAllHandle->Draw(MVP);
	}

	PHandle(float len, Shader* shader, float lineWidth, bool drawOver,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 allColor = glm::vec3(1, 1, 0),
		glm::vec3 xColor = glm::vec3(1,0,0), glm::vec3 yColor = glm::vec3(0,1,0), glm::vec3 zColor = glm::vec3(0,0,1))
		: IEntity(shader, allColor, drawOver, pos, rot, scale)
	{
		mXHandle = new PLine(glm::vec3(len*0.05f,0,0), glm::vec3(len,0,0), shader, xColor, lineWidth, drawOver);
		mYHandle = new PLine(glm::vec3(0,len*0.05f,0), glm::vec3(0,len,0), shader, yColor, lineWidth, drawOver);
		mZHandle = new PLine(glm::vec3(0,0,len*0.05f), glm::vec3(0,0,len), shader, zColor, lineWidth, drawOver);
		mAllHandle = new PWireCube(len*0.1f, shader, allColor, lineWidth*0.5f, drawOver);

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