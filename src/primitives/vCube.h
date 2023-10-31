#pragma once
#include "rendering/mesh.h"

class VCube : public Mesh
{
public:
	VCube(const std::string& name, const float size, Material* material,
		const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
		: Mesh(name, material)
	{
		float h = size * 0.5f;

		std::vector<glm::vec3> positions = {
			glm::vec3(-h, -h, -h ),
			glm::vec3(-h, -h,  h ),
			glm::vec3( h, -h,  h ),
			glm::vec3( h, -h, -h ),

			glm::vec3(-h,  h, -h ),
			glm::vec3(-h,  h,  h ),
			glm::vec3( h,  h,  h ),
			glm::vec3( h,  h, -h ),

			glm::vec3(-h, -h, -h ),
			glm::vec3(-h,  h, -h ),
			glm::vec3( h,  h, -h ),
			glm::vec3( h, -h, -h ),

			glm::vec3(-h, -h,  h ),
			glm::vec3(-h,  h,  h ),
			glm::vec3( h,  h,  h ),
			glm::vec3( h, -h,  h ),

			glm::vec3(-h, -h, -h ),
			glm::vec3(-h, -h,  h ),
			glm::vec3(-h,  h,  h ),
			glm::vec3(-h,  h, -h ),

			glm::vec3( h, -h, -h ),
			glm::vec3( h, -h,  h ),
			glm::vec3( h,  h,  h ),
			glm::vec3( h,  h, -h )
		};


		std::vector<glm::vec3> normals = {
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),

			glm::vec3(0.0f, 1.0f, 0.0f ),			
			glm::vec3(0.0f, 1.0f, 0.0f ),			
			glm::vec3(0.0f, 1.0f, 0.0f ),			
			glm::vec3(0.0f, 1.0f, 0.0f ),

			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),

			glm::vec3(0.0f, 0.0f, 1.0f ),			
			glm::vec3(0.0f, 0.0f, 1.0f ),			
			glm::vec3(0.0f, 0.0f, 1.0f ),			
			glm::vec3(0.0f, 0.0f, 1.0f ),

			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),

			glm::vec3(1.0f, 0.0f, 0.0f ),			
			glm::vec3(1.0f, 0.0f, 0.0f ),			
			glm::vec3(1.0f, 0.0f, 0.0f ),			
			glm::vec3(1.0f, 0.0f, 0.0f )
		};

		std::vector<glm::vec3> tangents = {
			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),

			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),

			glm::vec3( -1.0f, 0.0f, 0.0f ),
			glm::vec3( -1.0f, 0.0f, 0.0f ),
			glm::vec3( -1.0f, 0.0f, 0.0f ),
			glm::vec3( -1.0f, 0.0f, 0.0f ),

			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),
			glm::vec3( 1.0f, 0.0f, 0.0f ),

			glm::vec3( 0.0f, 0.0f, 1.0f ),
			glm::vec3( 0.0f, 0.0f, 1.0f ),
			glm::vec3( 0.0f, 0.0f, 1.0f ),
			glm::vec3( 0.0f, 0.0f, 1.0f ),

			glm::vec3( 0.0f, 0.0f, -1.0f ),
			glm::vec3( 0.0f, 0.0f, -1.0f ),
			glm::vec3( 0.0f, 0.0f, -1.0f ),
			glm::vec3( 0.0f, 0.0f, -1.0f )
		};

		std::vector<glm::vec2> texCoords = {
			glm::vec2( 0.0f, 0.0f ),
			glm::vec2( 0.0f, 1.0f ),
			glm::vec2( 1.0f, 1.0f ),
			glm::vec2( 1.0f, 0.0f ),

			glm::vec2( 0.0f, 1.0f ),
			glm::vec2( 0.0f, 0.0f ),
			glm::vec2( 1.0f, 0.0f ),
			glm::vec2( 1.0f, 1.0f ),

			glm::vec2( 1.0f, 0.0f ),
			glm::vec2( 1.0f, 1.0f ),
			glm::vec2( 0.0f, 1.0f ),
			glm::vec2( 0.0f, 0.0f ),

			glm::vec2( 0.0f, 0.0f ),
			glm::vec2( 0.0f, 1.0f ),
			glm::vec2( 1.0f, 1.0f ),
			glm::vec2( 1.0f, 0.0f ),

			glm::vec2( 0.0f, 0.0f ),
			glm::vec2( 1.0f, 0.0f ),
			glm::vec2( 1.0f, 1.0f ),
			glm::vec2( 0.0f, 1.0f ),

			glm::vec2( 1.0f, 0.0f ),
			glm::vec2( 0.0f, 0.0f ),
			glm::vec2( 0.0f, 1.0f ),
			glm::vec2( 1.0f, 1.0f )
		};

		std::vector<unsigned int> tempIndices = {
			0, 2, 1,
			0, 3, 2,
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11,
			12, 15, 14,
			12, 14, 13,
			16, 17, 18,
			16, 18, 19,
			20, 23, 22,
			20, 22, 21
		};

		// Generate verts of cube
		for (unsigned int i = 0; i < 24; i++)
			mVertices.push_back(Vertex(positions[i], normals[i], texCoords[i], tangents[i]));
		
		// Load indices
		mIndices.insert(mIndices.begin(), tempIndices.begin(), tempIndices.end());

		mPos = pos;
		mRot = rot;
		mScale = scale;

		Mesh::GenBuffers();
	}
};