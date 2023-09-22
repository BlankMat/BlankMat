#include "face.h"
#include "triangle.h"

// Adds the triangulated face to the given out parameter
void Face::GetTri(std::vector<Triangle>& tris, std::unordered_map<int, Vertex>& tempVerts, int _index)
{
	CalcCenter(tempVerts);
	CalcNormal(tempVerts);

	// Handle edge cases
	if (vertices.size() < 3) {
		return;
	}
	// Handle triangles
	else if (vertices.size() == 3) {
		tris.push_back(Triangle(vertices[0], vertices[1], vertices[2], normal, center, mat, shadingGroup, _index, TriTag::CONFIRMED_CORRECT));
	}
	// Handle quads
	else if (vertices.size() == 4) {
		float dist1 = glm::distance(tempVerts[vertices[0]].pos, tempVerts[vertices[2]].pos);
		float dist2 = glm::distance(tempVerts[vertices[1]].pos, tempVerts[vertices[3]].pos);
		if (dist1 > dist2) {
			tris.push_back(Triangle(vertices[0], vertices[1], vertices[3], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
			tris.push_back(Triangle(vertices[1], vertices[2], vertices[3], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
		}
		else {
			tris.push_back(Triangle(vertices[0], vertices[1], vertices[2], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
			tris.push_back(Triangle(vertices[0], vertices[2], vertices[3], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
		}
	}
	// Handle ngons
	else if (vertices.size() > 4) {
		for (int i = 0; i < vertices.size() - 2; i++) {
			if (i < vertices.size() - 3) {
				float dist1 = glm::distance(tempVerts[vertices[i]].pos, tempVerts[vertices[i + 2]].pos);
				float dist2 = glm::distance(tempVerts[vertices[i + 1]].pos, tempVerts[vertices[i + 3]].pos);

				if (dist1 > dist2) {
					tris.push_back(Triangle(vertices[i], vertices[i + 1], vertices[i + 3], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
					tris.push_back(Triangle(vertices[i + 1], vertices[i + 2], vertices[i + 3], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
				}
				else {
					tris.push_back(Triangle(vertices[i], vertices[i + 1], vertices[i + 2], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
					tris.push_back(Triangle(vertices[i], vertices[i + 2], vertices[i + 3], normal, center, mat, shadingGroup, _index, TriTag::UNCONFIRMED));
				}
				// If two tris were handled at once, skip the next one
				i++;
			}
			else {
				tris.push_back(GetClockwiseTri(
					IndVertex(vertices[i], tempVerts[vertices[i]]),
					IndVertex(vertices[i + 1], tempVerts[vertices[i + 1]]),
					IndVertex(vertices[i + 2], tempVerts[vertices[i + 2]]), mat, shadingGroup, _index));
			}
		}
	}
}

Triangle Face::GetClockwiseTri(IndVertex a, IndVertex b, IndVertex c, std::string _mat, int _sg, int _index)
{
	//if(IsInside(center + normal * 0.001f))
	//	return Triangle(a.id, b.id, c.id, normal, center, _mat, _sg);
	if (IsCCW(a.ver.pos, b.ver.pos, c.ver.pos)) {
		//std::cout << "Used triangulation method 1 (abc)" << std::endl;
		return Triangle(a.id, b.id, c.id, normal, center, _mat, _sg, _index, TriTag::UNCONFIRMED);
	}
	else {
		//std::cout << "Used triangulation method 2 (acb)" << std::endl;
		return Triangle(a.id, c.id, b.id, normal, center, _mat, _sg, _index, TriTag::UNCONFIRMED);
	}
}

int Face::GetNumVerts()
{
	return (int)vertices.size();
}

int Face::GetVertex(int _index)
{
	if (_index >= 0 && _index < vertices.size())
		return vertices[_index];
	else
		return -1;
}

void Face::SetVertex(int _vertex, int _index)
{
	// Do nothing for negative indices
	if (_index < 0)
		return;

	// Overwrite existing vertices
	if (_index > vertices.size()) {
		vertices[_index] = _vertex;
		return;
	}

	// If the vertex is past the end of the list, simply add it to the end
	vertices.push_back(_vertex);
}

void Face::SetVertices(std::vector<int>& _verts)
{
	Clear();
	for (int i = 0; i < _verts.size(); i++) {
		vertices.push_back(_verts[i]);
	}
}

void Face::SetMat(std::string _mat)
{
	mat = _mat;
}

void Face::SetCenter(glm::vec3 _center)
{
	center = _center;
}

void Face::SetNormal(glm::vec3 _norm)
{
	normal = _norm;
}

void Face::CalcCenter(std::unordered_map<int, Vertex>& _verts)
{
	center = (_verts[vertices[2]].pos + _verts[vertices[1]].pos + _verts[vertices[0]].pos) / 3.0f;
}

void Face::CalcNormal(std::unordered_map<int, Vertex>& _verts)
{
	normal = glm::vec3(0, 0, 0);
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 cur = _verts[vertices[i]].pos;
		glm::vec3 next = _verts[vertices[(i + 1) % vertices.size()]].pos;

		normal += glm::vec3((cur.y - next.y) * (cur.z + next.z), (cur.z - next.z) * (cur.x + next.x), (cur.x - next.x) * (cur.y + next.y));
	}
	normal = glm::normalize(normal);
}

void Face::Clear()
{
	vertices.clear();
}

Face::Face(std::string _mat, int _sg)
{
	mat = _mat;
	shadingGroup = _sg;
}

Face::Face(int i0, int i1, int i2, std::string _mat, int _sg)
{
	mat = _mat;
	shadingGroup = _sg;
	vertices.push_back(i0);
	vertices.push_back(i1);
	vertices.push_back(i2);
}

Face::Face(int i0, int i1, int i2, int i3, std::string _mat, int _sg)
{
	mat = _mat;
	shadingGroup = _sg;
	vertices.push_back(i0);
	vertices.push_back(i1);
	vertices.push_back(i2);
	vertices.push_back(i3);
}

Face::Face(std::vector<int>& _verts, std::string _mat, int _sg)
{
	mat = _mat;
	shadingGroup = _sg;
	for (int i = 0; i < _verts.size(); i++) {
		vertices.push_back(_verts[i]);
	}
}

Face::Face(std::vector<IndVertex>& _verts, std::string _mat, int _sg)
{
	mat = _mat;
	shadingGroup = _sg;
	for (int i = 0; i < _verts.size(); i++) {
		vertices.push_back(_verts[i].id);
	}
}

Face::~Face()
{
	Clear();
}
