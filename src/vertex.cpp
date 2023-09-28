#include "vertex.h"

Vertex::Vertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _texCoords)
{
	pos = _pos;
	normal = _normal;
	texCoords = _texCoords;
}