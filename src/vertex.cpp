#include "vertex.h"

Vertex::Vertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec2 _texCoords, glm::vec3 _text)
{
	pos = _pos;
	normal = _normal;
	texCoords = _texCoords;
	texture = _text;
}