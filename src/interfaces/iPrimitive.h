#pragma once
#include "glIncludes.h"
#include "interfaces/iEntity.h"
#include "rendering/shader.h"
#include <vector>

template <typename V, typename I>
class IPrimitive : public IEntity
{
protected:
	float mLineWidth;
	bool mIsWireframe;
	unsigned int mIndexLen;

	std::vector<V> mVertices;
	std::vector<I> mIndices;

	// Generates the necessary VAO, VBO, and EBO for the object after verts and indices are set
	void GenBuffers() override
	{
		mVAO = mVBO = mEBO = 0;
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, (GLuint)mVertices.size() * sizeof(*mVertices.data()), mVertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &mEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLuint)mIndices.size() * sizeof(*mIndices.data()), mIndices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		mIndexLen = (GLuint)mIndices.size() * sizeof(*mIndices.data());
	}
public:
	// Draws the object to the screen
	void Draw(Shader* shader, State* state, Material* defaultMat, glm::mat4 viewProj, glm::mat4 model = glm::mat4(1.0f)) override
	{
		// Don't draw disabled objects
		if (!mIsEnabled)
			return;

		// Set uniforms for this draw
		glm::mat4 modelMatrix = model * GetModelMatrix();
		glm::mat4 mvp = viewProj * modelMatrix;
		glm::mat3 normalModel = GetNormalModelMatrix(model);
		mRecalcMatrices = false;

		shader->SetMat4("MVP", mvp);
		shader->SetMat4("Model", modelMatrix);
		shader->SetMat3("NormalModel", normalModel);

		// Bind shadow map to next available texture index
		unsigned int shadowIndex = mMaterial->UpdateShader(shader, state, defaultMat);
		glActiveTexture(GL_TEXTURE0 + shadowIndex);
		glBindTexture(GL_TEXTURE_2D, state->depthMap);
		shader->SetInt("shadowMap", state->depthMapFBO);
		glActiveTexture(GL_TEXTURE0);

		if (mDrawOver)
			glDisable(GL_DEPTH_TEST);
		glBindVertexArray(mVAO);
		if (mIsWireframe)
			glLineWidth(mLineWidth);
		glDrawElements(mIsWireframe ? GL_LINES : GL_TRIANGLES, (GLuint)mIndexLen, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}
	
	// Draws the object's shadows
	void DrawShadows(Shader* shader, State* state, glm::mat4 model = glm::mat4(1.0f)) override {}

	IPrimitive(std::string name, Material* material = nullptr, float lineWidth = 0.1f, bool drawOver = false,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: IEntity(name, material, drawOver, pos, rot, scale), mLineWidth(lineWidth)
	{
		// If the lineWidth is positive, draw wireframe
		mIsWireframe = lineWidth > 0.0f;
		mIndexLen = 0;
	}
};