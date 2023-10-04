#pragma once
#include "glIncludes.h"
#include "options.h"
#include "iEntity.h"

class ILight : public IEntity
{
protected:
	glm::vec3 mBaseColor;
	glm::vec3 mOffset;
	glm::vec3 mDir;
	float m_ka;
	float m_ks;
	bool mGamma;

	void GenBuffers() override { mVAO = mVBO = mEBO = 0; }
public:
	void Draw(glm::mat4 viewProj) override {}

	// Updates the lighting values of the given shader
	void UpdateShader(Shader* shader)
	{
		shader->SetVec3("light.diffuse", mColor);
		shader->SetVec3("light.ambient", mColor * m_ka);
		shader->SetVec3("light.specular", glm::vec3(1.0f) * m_ks);
		shader->SetVec3("light.position", mPos);
		shader->SetBool("gamma", mGamma);
	}

	glm::vec3 GetBaseColor() { return mBaseColor; }
	glm::vec3 GetOffset() { return mOffset; }
	glm::vec3 GetDir() { return mDir; }
	float GetKA() { return m_ka; }
	float GetKS() { return m_ks; }

	void SetOffset(glm::vec3 offset) { mOffset = offset; }
	void SetBaseColor(glm::vec3 color) { mBaseColor = color; }

	ILight(glm::vec3 pos = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(-1.0f), glm::vec3 color = glm::vec3(1.0f), 
		float ka = 0.1f, float ks = 0.5f, bool gamma = true) 
		: IEntity(nullptr, color, false, pos), mBaseColor(color), mDir(dir), m_ka(ka), m_ks(ks), mOffset(pos), mGamma(gamma) {}
	ILight(Options* options)
		: ILight(options->lightPos, options->lightDir, options->lightColor, options->lightKA, options->lightKS, options->gamma) {}
};