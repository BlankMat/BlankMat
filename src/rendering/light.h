#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "shader.h"
#include "material.h"

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
	float mLightRange;

	// Spotlight variables
	float mSpotInner;
	float mSpotOuter;

	// Shadow mapping variables
	glm::mat4 mLightSpace;
	float mNearPlane;
	float mFarPlane;
	float mLightSize;

	void CalcMatrices()
	{
		mNearPlane = mLightRange * 0.1f;
		mFarPlane = mLightRange * 10.0f;
		glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
		if (mType == LightType::DIR || mType == LightType::SPOT)
			target = mPos + mDir;
		glm::mat4 mLightProj = glm::ortho(-mLightSize, mLightSize, -mLightSize, mLightSize, mNearPlane, mFarPlane);
		glm::mat4 mLightView = glm::lookAt(mPos, target, glm::vec3(0,1,0));
		mLightSpace = mLightProj * mLightView;
	}
public:
	// Draws the object to the screen
	virtual void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats = false) {}

	// Updates the lighting values of the given shader
	void UpdateShader(Shader* shader)
	{
		// Render light
		shader->SetVec3("light.diffuse", mColor * mKD);
		shader->SetVec3("light.ambient", mColor * mKA);
		shader->SetVec3("light.specular", glm::vec3(1.0f) * mKS);
		shader->SetVec3("light.position", mPos);
		shader->SetVec3("light.direction", mDir);
		shader->SetBool("light.gamma", mGamma);
		shader->SetInt("light.type", (int)mType);
		shader->SetMat4("LightSpace", mLightSpace);

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
	float GetRange() { return mLightRange; }
	float GetSpotInnerRadius() { return mSpotInner; }
	float GetSpotOuterRadius() { return mSpotOuter; }

	virtual void SetColor(const glm::vec3& color) { mColor = color; }
	virtual void SetBaseColor(const glm::vec3& color) { mBaseColor = color; }
	virtual void SetOffset(const glm::vec3& offset) { mOffset = offset; CalcMatrices(); }
	virtual void SetPos(const glm::vec3& pos) { mPos = pos; CalcMatrices(); }
	virtual void SetDir(const glm::vec3& dir) { mDir = dir; CalcMatrices(); }
	virtual void SetKD(const float kd) { mKD = kd; }
	virtual void SetKA(const float ka) { mKA = ka; }
	virtual void SetKS(const float ks) { mKS = ks; }
	virtual void SetGamma(const bool gamma) { mGamma = gamma; }
	virtual void SetSpotInnerRadius(const float innerRadius) { mSpotInner = innerRadius; }
	virtual void SetSpotOuterRadius(const float outerRadius) { mSpotOuter = outerRadius; }

	virtual void SetType(const LightType type)
	{
		mType = type;
		CalcMatrices();
	}

	void UpdateRotatingLight(bool isRotating)
	{
		if (!isRotating)
			SetPos(mOffset);
		else
			SetPos(glm::vec3(mOffset.x * sin((float)glfwGetTime()), mOffset.y, mOffset.z * cos((float)glfwGetTime())));
	}

	void UpdateDiscoLight(bool isDisco)
	{
		if (!isDisco)
			SetColor(mBaseColor);
		else
			SetColor(glm::vec3(mBaseColor.x * sin((float)glfwGetTime() - PI * 0.5f), mBaseColor.y * sin((float)glfwGetTime()), mBaseColor.z * sin((float)glfwGetTime() + PI * 0.5f)));
	}

	void SetRange(const float lightRange)
	{
		mLightRange = (lightRange > 0.0f) ? lightRange : 0.1f;
		mPointC = 1.0f;
		mPointL = 0.47f / mLightRange;
		mPointQ = 8.5f / pow(mLightRange, 2.0f);
		CalcMatrices();
	}

	Light(LightType type = LightType::POINT, const glm::vec3& pos = glm::vec3(1.0f), const glm::vec3& dir = glm::vec3(-1.0f), 
		const glm::vec3& color = glm::vec3(1.0f), float kd = 1.0f, float ka = 0.1f, float ks = 0.5f, bool gamma = true, 
		float range = 13.0f, float spotInner = 25, float spotOuter = 35)
		: mType(type), mColor(color), mBaseColor(color), mDir(dir), mPos(pos), mKD(kd), mKA(ka), mKS(ks), mOffset(pos), mGamma(gamma), 
		mSpotInner(spotInner), mSpotOuter(spotOuter)
	{
		mLightSize = 10.0f;
		SetRange(range);
		CalcMatrices();
	}

	Light(Config* config)
		: Light(static_cast<LightType>(config->GetInt("type")), config->GetVec("pos"), config->GetVec("dir"), 
			config->GetVec("color"), config->GetFloat("diffuse"), config->GetFloat("ambient"), config->GetFloat("specular"), 
			config->GetBool("gamma"), config->GetFloat("range"), config->GetFloat("spotInnerRadius"), config->GetFloat("spotOuterRadius")) {}
};