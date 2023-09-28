#pragma once
#include "line.h"
#include "drawable.h"

class TransformHandle : public Drawable {
private:
	Line* xHandle;
	Line* yHandle;
	Line* zHandle;

	glm::vec3 pos;

public:

	void Draw(glm::mat4 viewProj)
	{
		glm::mat4 MVP = viewProj * glm::translate(glm::mat4(1.0f), pos);
		xHandle->Draw(MVP);
		yHandle->Draw(MVP);
		zHandle->Draw(MVP);
	}

	TransformHandle(glm::vec3 _pos, float width, float len, Shader* shader, 
		glm::vec3 xColor = glm::vec3(1,0,0), glm::vec3 yColor = glm::vec3(0,1,0), glm::vec3 zColor = glm::vec3(0,0,1))
	{
		xHandle = new Line(glm::vec3(0), glm::vec3(len,0,0), xColor, width, shader);
		yHandle = new Line(glm::vec3(0), glm::vec3(0,len,0), yColor, width, shader);
		zHandle = new Line(glm::vec3(0), glm::vec3(0,0,len), zColor, width, shader);
		pos = _pos;
	}

	~TransformHandle()
	{
		delete xHandle;
		delete yHandle;
		delete zHandle;
	}
};