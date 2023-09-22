#pragma once
#include "glIncludes.h"
#include "texture.h"
#include "vertex.h"
#include "shader.h"
#include <vector>

class Mesh
{
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader& shader);
};