#pragma once
#include "glIncludes.h"
#include "drawable.h"
#include "shader.h"
#include <vector>

template <typename V, typename I>
class Primitive : public Drawable
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
	virtual void Draw(glm::mat4 viewProj)
	{
		mShader->Use();
		mShader->SetMat4("MVP", viewProj * GetModelMatrix());
		mShader->SetVec3("Color", mColor);

		glBindVertexArray(mVAO);
		if (mIsWireframe)
			glLineWidth(mLineWidth);
		glDrawElements(mIsWireframe ? GL_LINES : GL_TRIANGLES, (GLuint)mIndexLen, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}

	Primitive(Shader* shader = nullptr, glm::vec3 color = glm::vec3(), float lineWidth = 0.1f,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: Drawable(shader, color, pos, rot, scale), mLineWidth(lineWidth)
	{
		// If the lineWidth is positive, draw wireframe
		mIsWireframe = lineWidth > 0.0f;
		mIndexLen = 0;
	}
};