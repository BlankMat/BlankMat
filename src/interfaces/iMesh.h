#pragma once
#include "glIncludes.h"
#include "interfaces/iPrimitive.h"
#include "rendering/vertex.h"
#include <vector>
#include "utils.h"

#define ACCEPTABLE_MARGIN_OF_ERROR 0.0001

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

	// Returns the face ID that collides with the provided line
	// Returns -1 if the line does not collide with the mesh
	int CheckLineFaceCollision(glm::vec3 localPoint,glm::vec3 localVector)
	{
		int resultFaceID = -1;
		for(int i = 0; i < mIndices.size(); i = i + 3)
		{
			// Get the points of a triangular face
			glm::vec3 localPoint1 = mVertices[mIndices[i]].pos;
			glm::vec3 localPoint2 = mVertices[mIndices[i+1]].pos;
			glm::vec3 localPoint3 = mVertices[mIndices[i+2]].pos;

			glm::vec3 localVector12 = localPoint1 - localPoint2;
			glm::vec3 localVector23 = localPoint2 - localPoint3;
			glm::vec3 localVector31 = localPoint3 - localPoint1;

			glm::vec3 planeNormalVector = (glm::cross(localVector12,-localVector23));

			// Formula of a plane with normal vector <a,b,c> is ax+by+cz+d = 0
			float xMultiplier = planeNormalVector.x;
			float yMultiplier = planeNormalVector.y;
			float zMultiplier = planeNormalVector.z;
			float dConstant = glm::dot(planeNormalVector,localPoint1) * -1;

			float totalConstant = dConstant;
			float totalParametric = 0;

			totalConstant += xMultiplier*localPoint[0];
			totalConstant += yMultiplier*localPoint[1];
			totalConstant += zMultiplier*localPoint[2];

			totalParametric += xMultiplier*localVector[0];
			totalParametric += yMultiplier*localVector[1];
			totalParametric += zMultiplier*localVector[2];

			// If the parametric portion is 0 and the constant portion is non-zero, 
			// the line does not intersect the plane.
			// The situation where a line is completely within a plane is ignored as
			// that would imply that the face is perfectly parellel from the camera.
			if(totalParametric == 0)
			{
				continue;
			}
			
			int parametricSolution = (totalConstant/totalParametric) * -1;
			glm::vec3 intersectPoint = glm::vec3
			(parametricSolution*localVector.x,
			parametricSolution*localVector.y,
			parametricSolution*localVector.z) + localPoint;

			// Determine if the point is within the triangle
			glm::vec3 localVector1I = localPoint1 - intersectPoint;
			glm::vec3 localVector2I = localPoint2 - intersectPoint;
			glm::vec3 localVector3I = localPoint3 - intersectPoint;

			glm::vec3 normalVector1 = glm::normalize(glm::cross(localVector12,localVector1I));
			glm::vec3 normalVector2 = glm::normalize(glm::cross(localVector23,localVector2I));
			glm::vec3 normalVector3 = glm::normalize(glm::cross(localVector31,localVector3I));

			planeNormalVector = glm::normalize(planeNormalVector);

			glm::vec3 normalVectors12Contrast = normalVector1-normalVector2;
			glm::vec3 normalVectors23Contrast = normalVector2-normalVector3;
			if(Vec3FloatEqual(normalVector1,normalVector2,ACCEPTABLE_MARGIN_OF_ERROR) && 
			Vec3FloatEqual(normalVector2,normalVector3,ACCEPTABLE_MARGIN_OF_ERROR) &&
			(Vec3FloatEqual(normalVector1,planeNormalVector,ACCEPTABLE_MARGIN_OF_ERROR)||
			Vec3FloatEqual(normalVector1,-planeNormalVector,ACCEPTABLE_MARGIN_OF_ERROR)))
			{
				resultFaceID = i/3;
				break;
			}
		}
		return resultFaceID;
	}

	IMesh(std::string name, Material* material = nullptr,
		glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
		: IPrimitive(name, material, 0.0f, false, pos, rot, scale)
	{
		CalcBasis();
	}
};