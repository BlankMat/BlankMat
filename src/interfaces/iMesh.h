#pragma once
#include "glIncludes.h"
#include "interfaces/iPrimitive.h"
#include "rendering/vertex.h"
#include <vector>

class IMesh : public IPrimitive<Vertex, unsigned int>
{
protected:
	// Generates the required buffers to render the mesh
	void GenBuffers() override
	{
		mVAO = mVBO = mEBO = 0;
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		glBufferData(GL_ARRAY_BUFFER, (GLsizei)mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		// vertex tangents
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glBindVertexArray(0);
		mIndexLen = (GLuint)mIndices.size() * sizeof(*mIndices.data());
	}
public:
	// Returns the vertex with the given index
	Vertex* GetVertex(unsigned int index)
	{
		if (index < mVertices.size())
			return nullptr;
		return &mVertices[index];
	}

	IMesh(std::string name, Material* material = nullptr,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: IPrimitive(name, material, 0.0f, false, pos, rot, scale)
	{
		CalcBasis();
	}
};