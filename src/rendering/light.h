#pragma once
#include "glIncludes.h"
#include "files/config.h"

enum class LightType { POINT = 0, DIR = 1, SPOT = 2 };

class Light
{
protected:
	glm::vec3 mColor;
	glm::vec3 mBaseColor;
	glm::vec3 mOffset;
	glm::vec3 mDir;
	glm::vec3 mPos;
	LightType mType;

	// Light intensity info
	float mKD;
	float mKA;
	float mKS;
	bool mGamma;

	// Point light variables
	float mPointC;
	float mPointL;
	float mPointQ;
	float mPointRange;

	// Spotlight variables
	float mSpotInner;
	float mSpotOuter;
public:
	// Updates the lighting values of the given shader
	void UpdateShader(Shader* shader)
	{
		shader->SetVec3("light.diffuse", mColor * mKD);
		shader->SetVec3("light.ambient", mColor * mKA);
		shader->SetVec3("light.specular", glm::vec3(1.0f) * mKS);
		shader->SetVec3("light.position", mPos);
		shader->SetVec3("light.direction", mDir);
		shader->SetBool("light.gamma", mGamma);
		shader->SetInt("light.type", (int)mType);

		// Point light
		if (mType == LightType::POINT)
		{
			shader->SetFloat("light.pointC", mPointC);
			shader->SetFloat("light.pointL", mPointL);
			shader->SetFloat("light.pointQ", mPointQ);
		}
		// Spotlight
		else if (mType == LightType::SPOT)
		{
			shader->SetFloat("light.spotInner", glm::cos(glm::radians(mSpotInner)));
			shader->SetFloat("light.spotOuter", glm::cos(glm::radians(mSpotOuter)));
		}
	}

	LightType GetType() { return mType; }
	glm::vec3 GetColor() { return mColor; }
	glm::vec3 GetBaseColor() { return mBaseColor; }
	glm::vec3 GetOffset() { return mOffset; }
	glm::vec3 GetDir() { return mDir; }
	glm::vec3 GetPos() { return mPos; }
	float GetKD() { return mKD; }
	float GetKA() { return mKA; }
	float GetKS() { return mKS; }
	bool GetGamma() { return mGamma; }
	float GetPointRange() { return mPointRange; }
	float GetSpotInnerRadius() { return mSpotInner; }
	float GetSpotOuterRadius() { return mSpotOuter; }

	void SetType(LightType type) { mType = type; }
	void SetColor(glm::vec3 color) { mColor = color; }
	void SetBaseColor(glm::vec3 color) { mBaseColor = color; }
	void SetOffset(glm::vec3 offset) { mOffset = offset; }
	void SetPos(glm::vec3 pos) { mPos = pos; }
	void SetDir(glm::vec3 dir) { mDir = dir; }
	void SetKD(float kd) { mKD = kd; }
	void SetKA(float ka) { mKA = ka; }
	void SetKS(float ks) { mKS = ks; }
	void SetGamma(bool gamma) { mGamma = gamma; }
	void SetSpotInnerRadius(float innerRadius) { mSpotInner = innerRadius; }
	void SetSpotOuterRadius(float outerRadius) { mSpotOuter = outerRadius; }

	void SetPointRange(float pointRange)
	{
		if (pointRange <= 0.0f)
			pointRange = 0.001f;
		mPointRange = pointRange;
		mPointC = 1.0f;
		mPointL = 4.7f / pointRange;
		mPointQ = 85.0f / pow(pointRange, 2.0f);
	}

	Light(LightType type = LightType::POINT, glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f), 
		glm::vec3 color = glm::vec3(1.0f), float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, bool gamma = true, 
		float pointRange = 13.0f, float spotInner = 25, float spotOuter = 35)
		: mType(type), mColor(color), mBaseColor(color), mDir(dir), mPos(pos), mKD(kd), mKA(ka), mKS(ks), mOffset(pos), mGamma(gamma), 
		mSpotInner(spotInner), mSpotOuter(spotOuter)
	{
		SetPointRange(pointRange);
	}

	Light(Config* config)
		: Light((LightType)config->GetInt("type"), config->GetVec("pos"), config->GetVec("dir"), 
			config->GetVec("color"), config->GetFloat("diffuse"), config->GetFloat("ambient"), config->GetFloat("specular"), 
			config->GetBool("gamma"), config->GetFloat("pointRange"), config->GetFloat("spotInnerRadius"), config->GetFloat("spotOuterRadius")) {}
};