#pragma once
#include "glIncludes.h"
#include "files/config.h"

class Light
{
protected:
	glm::vec3 mColor;
	glm::vec3 mBaseColor;
	glm::vec3 mOffset;
	glm::vec3 mDir;
	glm::vec3 mPos;

	float mKD;
	float mKA;
	float mKS;
	bool mGamma;
public:
	// Updates the lighting values of the given shader
	void UpdateShader(Shader* shader)
	{
		shader->SetVec3("light.diffuse", mColor * mKD);
		shader->SetVec3("light.ambient", mColor * mKA);
		shader->SetVec3("light.specular", glm::vec3(1.0f) * mKS);
		shader->SetVec3("light.position", mPos);
		shader->SetBool("gamma", mGamma);
	}

	glm::vec3 GetColor() { return mColor; }
	glm::vec3 GetBaseColor() { return mBaseColor; }
	glm::vec3 GetOffset() { return mOffset; }
	glm::vec3 GetDir() { return mDir; }
	glm::vec3 GetPos() { return mPos; }
	float GetKD() { return mKD; }
	float GetKA() { return mKA; }
	float GetKS() { return mKS; }
	bool GetGamma() { return mGamma; }

	void SetColor(glm::vec3 color) { mColor = color; }
	void SetBaseColor(glm::vec3 color) { mBaseColor = color; }
	void SetOffset(glm::vec3 offset) { mOffset = offset; }
	void SetPos(glm::vec3 pos) { mPos = pos; }
	void SetKD(float kd) { mKD = kd; }
	void SetKA(float ka) { mKA = ka; }
	void SetKS(float ks) { mKS = ks; }
	void SetGamma(bool gamma) { mGamma = gamma; }

	Light(glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f), glm::vec3 color = glm::vec3(1.0f), 
		float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, bool gamma = true) 
		: mColor(color), mBaseColor(color), mDir(dir), mPos(pos), mKD(kd), mKA(ka), mKS(ks), mOffset(pos), mGamma(gamma) {}
	Light(Config* config)
		: Light(config->GetVec("pos"), config->GetVec("dir"), config->GetVec("color"), config->GetFloat("ambient"), config->GetFloat("specular"), config->GetBool("gamma")) {}
};