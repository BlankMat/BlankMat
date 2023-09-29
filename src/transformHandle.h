#pragma once
#include "drawable.h"
#include "primitive.h"
#include "line.h"
#include "wireCube.h"

class TransformHandle : public Drawable
{
protected:
	Line* mXHandle;
	Line* mYHandle;
	Line* mZHandle;
	WireCube* mAllHandle;

	bool mXHeld = false;
	bool mYHeld = false;
	bool mZHeld = false;
	bool mAllHeld = false;

	void GenBuffers() override {}
public:

	void Draw(glm::mat4 viewProj)
	{
		glDisable(GL_DEPTH_TEST);
		glm::mat4 MVP = viewProj * Drawable::GetModelMatrix();
		if (!(mYHeld || mZHeld || mAllHeld))
			mXHandle->Draw(MVP);
		if (!(mXHeld || mZHeld || mAllHeld))
			mYHandle->Draw(MVP);
		if (!(mXHeld || mYHeld || mAllHeld))
			mZHandle->Draw(MVP);
		if (!(mXHeld || mYHeld || mZHeld))
			mAllHandle->Draw(MVP);
		glEnable(GL_DEPTH_TEST);
	}

	TransformHandle(float len, Shader* shader, float lineWidth,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 allColor = glm::vec3(1, 1, 0),
		glm::vec3 xColor = glm::vec3(1,0,0), glm::vec3 yColor = glm::vec3(0,1,0), glm::vec3 zColor = glm::vec3(0,0,1))
		: Drawable(shader, allColor, pos, rot, scale)
	{
		mXHandle = new Line(glm::vec3(len*0.05f,0,0), glm::vec3(len,0,0), shader, xColor, lineWidth);
		mYHandle = new Line(glm::vec3(0,len*0.05f,0), glm::vec3(0,len,0), shader, yColor, lineWidth);
		mZHandle = new Line(glm::vec3(0,0,len*0.05f), glm::vec3(0,0,len), shader, zColor, lineWidth);
		mAllHandle = new WireCube(len*0.1f, shader, allColor, lineWidth*0.5f);

		mXHeld = false;
		mYHeld = false;
		mZHeld = false;
		mAllHeld = false;
	}

	~TransformHandle()
	{
		delete mXHandle;
		delete mYHandle;
		delete mZHandle;
		delete mAllHandle;
	}
};