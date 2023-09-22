#include "mesh.h"
#include "openGLHelper.h"

void Mesh::CalcBasis()
{
	forward = glm::vec3(
		cos(rotation.y) * sin(rotation.x),
		sin(rotation.y),
		cos(rotation.y) * cos(rotation.x)
	);

	// Right vector
	right = glm::vec3(
		sin(rotation.x - HALF_PI),
		0,
		cos(rotation.x - HALF_PI)
	);

	// Up vector : perpendicular to both direction and right
	up = glm::cross(right, forward);
}

void Mesh::CalcPivot()
{
	pivot = glm::vec3(0, 0, 0);
	for (auto iter = verts.begin(); iter != verts.end(); ++iter) {
		pivot += iter->second.pos;
	}
	pivot /= (float)verts.size();
}

glm::vec3 Mesh::GetUp() { return up; }
glm::vec3 Mesh::GetRight() { return right; }
glm::vec3 Mesh::GetForward() { return forward; }

void Mesh::Translate(glm::vec3 _deltaPos) { pos += _deltaPos; }
void Mesh::Rotate(glm::vec3 _deltaRot) { rotation += _deltaRot; }
void Mesh::Scale(glm::vec3 _deltaScale) { scale = glm::max(glm::vec3(scale.x * _deltaScale.x, scale.y * _deltaScale.y, scale.z * _deltaScale.z), MIN_SCALE); }

glm::vec3 Mesh::GetPos() { return pos; }
glm::vec3 Mesh::GetRotation() { return rotation; }
glm::vec3 Mesh::GetScale() { return scale; }
glm::vec3 Mesh::GetPivot() { return pivot; }

void Mesh::SetPos(glm::vec3 _pos) { pos = _pos; }
void Mesh::SetRotation(glm::vec3 _rot) { rotation = _rot; }
void Mesh::SetScale(glm::vec3 _scale) { scale = glm::max(_scale, MIN_SCALE); }

// Translate the given verts by the given vector
void Mesh::Translate(std::set<int>& _verts, glm::vec3 _deltaPos)
{
	for (auto iter = _verts.begin(); iter != _verts.end(); ++iter) {
		verts[*iter].pos += _deltaPos;
	}
}

// Rotates the given verts by the given vector
void Mesh::Rotate(std::set<int>& _verts, glm::vec3 _deltaRot, glm::vec3 _pivot)
{
	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(_deltaRot.x), glm::vec3(0, 1, 0));
	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(_deltaRot.y), glm::vec3(1, 0, 0));
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(_deltaRot.z), glm::vec3(0, 0, 1));
	glm::mat4 rotMat = rotateZ * rotateY * rotateX;

	for (auto iter = _verts.begin(); iter != _verts.end(); ++iter) {
		verts[*iter].pos = RotateAround(glm::vec4(verts[*iter].pos, 1.0f), glm::vec4(_pivot, 1.0f), rotMat);
	}
}

// Scales the given verts by the given vector
void Mesh::Scale(std::set<int>& _verts, glm::vec3 _deltaScale, glm::vec3 _pivot)
{
	for (auto iter = _verts.begin(); iter != _verts.end(); ++iter) {
		glm::vec3 tempPos = verts[*iter].pos - _pivot;
		tempPos *= _deltaScale;
		verts[*iter].pos = tempPos + _pivot;
	}
}
// Mesh main function definitions
// ------------------------------
void Mesh::AddFace(Face _face) { faces.push_back(_face); }
int Mesh::AddVert(Vertex _vert) { return AddVert(lastVertIndex + 1, _vert); }

int Mesh::AddVert(int _index, Vertex _vert)
{
	// Only add the vertex if it doesn't exist in the mesh yet
	if (verts.find(_index) == verts.end()) {
		verts.emplace(_index, _vert);
		lastVertIndex = std::max(lastVertIndex, _index);
	}
	return _index;
}

Face Mesh::GetFace(size_t index) { return faces[index]; }
Vertex Mesh::GetVert(size_t index) { return verts[(int)index]; }

void Mesh::Clear()
{
	faces.clear();
	verts.clear();
	lastVertIndex = -1;
}

int Mesh::GetVertCount()
{
	return (int)verts.size();
}

int Mesh::GetIndexCount()
{
	int count = 0;
	for (int i = 0; i < faces.size(); i++) {
		// ngon will create (n-2) triangles with 3 vertices each
		count += (faces[i].GetNumVerts() - 2) * 3;
	}
	return count;
}

std::vector<Face>& Mesh::GetFaces()
{
	return faces;
}

std::vector<Triangle>& Mesh::GetTris(std::vector<Triangle>& _tris)
{
	// Convert all stored faces into tris
	for (int i = 0; i < faces.size(); i++) {
		faces[i].GetTri(_tris, verts, i);
	}
	return _tris;
}

std::vector<std::string>& Mesh::GetMatsForVert(std::vector<std::string>& _mats, int vertId)
{
	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < TRI_VERTS; j++) {
			if (faces[i].vertices[j] == vertId) {
				_mats.push_back(faces[i].mat);
				break;
			}
		}
	}
	return _mats;
}

#include <iostream>
void Mesh::ReorientTris(std::vector<Triangle>& _tris)
{
	for (int i = 0; i < _tris.size(); i++) {
		if (glm::dot(_tris[i].normal, _tris[i].center) < 0.0f) {
			std::cout << "Reoriented triangle [" << i <<
				"] Normal: [" << _tris[i].normal.x << ", " << _tris[i].normal.y << ", " << _tris[i].normal.z <<
				"], Center: [" << _tris[i].center.x << ", " << _tris[i].center.y << ", " << _tris[i].center.z << "]" << std::endl;
			_tris[i] = Triangle(_tris[i].vertices[0], _tris[i].vertices[2], _tris[i].vertices[1], -_tris[i].normal, _tris[i].center, _tris[i].mat, _tris[i].shadingGroup);
		}
	}
}

std::unordered_map<int, Vertex>& Mesh::GetVerts()
{
	return verts;
}

std::string Mesh::GetName() { return name; }
void Mesh::SetName(std::string _name) { name = _name; }

void Mesh::RecalculateNormals()
{
	// Reset normals for each tri
	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < faces[i].GetNumVerts(); j++) {
			verts[faces[i].vertices[j]].normal = glm::vec3(0, 0, 0);
		}
	}

	// Calculate normals for each tri
	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < faces[i].GetNumVerts(); j++) {
			glm::vec3 curVert = verts[faces[i].vertices[j]].pos;
			glm::vec3 nextVert = verts[faces[i].vertices[(j + 1) % faces[i].GetNumVerts()]].pos;
			verts[faces[i].vertices[j]].normal += glm::vec3(
				(curVert.y - nextVert.y) * (curVert.z + nextVert.z),
				(curVert.z - nextVert.z) * (curVert.x + nextVert.x),
				(curVert.x - nextVert.x) * (curVert.y + nextVert.y));
		}
	}

	// Normalize normals for each vertex
	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < faces[i].GetNumVerts(); j++) {
			verts[faces[i].vertices[j]].normal = glm::normalize(verts[faces[i].vertices[j]].normal);
		}
		// Calculate face normal
		faces[i].CalcNormal(verts);
	}
}

int Mesh::GetVertexModel() { return 1; }

Mesh::Mesh()
{
	pos = glm::vec3();
}

Mesh::~Mesh()
{
	Clear();
}