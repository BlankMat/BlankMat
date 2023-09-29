#pragma once
#include "glIncludes.h"
#include "drawable.h"

class Light : public Drawable
{
protected:
	glm::vec3 mOffset;
	glm::vec3 mDir;
	float m_ka;
	float m_ks;

	void GenBuffers() override {}
public:
	glm::vec3 GetColor() { return mColor; }
	glm::vec3 GetPos() { return mPos; }
	glm::vec3 GetOffset() { return mOffset; }
	glm::vec3 GetDir() { return mDir; }
	float GetKA() { return m_ka; }
	float GetKS() { return m_ks; }

	void Draw(glm::mat4 viewProj) {}

	Light(glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f), glm::vec3 color = glm::vec3(1.0f), 
		float ka = 0.1f, float ks = 0.5f) : Drawable(nullptr, color, pos), mDir(dir), m_ka(ka), m_ks(ks), mOffset(pos) {}
};