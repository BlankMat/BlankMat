#pragma once
#include "triangle.h"
#include "face.h"
#include "mathLib.h"
#include <vector>
#include <unordered_map>
#include <set>
const float MIN_SCALE = 0.0000001f;

class OldMesh {
protected:
	// Direct values
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	// Calculated values
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 forward = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 pivot = glm::vec3(0, 0, 0);

	// Data
	std::string name;
	std::vector<Face> faces;
	std::unordered_map<int, Vertex> verts;
public:
	Material defaultMat = Material();
	int lastVertIndex = -1;

	// Calculates the basis of the mesh
	void CalcBasis();
	// Calculates the pivot of the mesh
	void CalcPivot();

	// Getter functions
	// Returns the up vector of this mesh
	glm::vec3 GetUp();
	// Returns the right vector of this mesh
	glm::vec3 GetRight();
	// Returns the forward vector of this mesh
	glm::vec3 GetForward();
	// Returns the position of the mesh
	glm::vec3 GetPos();
	// Returns the rotation of the mesh
	glm::vec3 GetRotation();
	// Returns the scale of the mesh
	glm::vec3 GetScale();
	// Returns the pivot of the mesh
	glm::vec3 GetPivot();

	// Setter functions
	// Translate the mesh by the given vector
	void Translate(glm::vec3 _deltaPos);
	// Rotates the mesh by the given vector
	void Rotate(glm::vec3 _deltaRot);
	// Scales the mesh by the given vector
	void Scale(glm::vec3 _deltaScale);
	// Translate the given verts by the given vector
	void Translate(std::set<int>& _verts, glm::vec3 _deltaPos);
	// Rotates the given verts by the given vector
	void Rotate(std::set<int>& _verts, glm::vec3 _deltaRot, glm::vec3 _pivot);
	// Scales the given verts by the given vector
	void Scale(std::set<int>& _verts, glm::vec3 _deltaScale, glm::vec3 _pivot);
	// Sets the position of the mesh to the given position
	void SetPos(glm::vec3 _pos);
	// Sets the rotation of the mesh to the given rotation
	void SetRotation(glm::vec3 _rot);
	// Sets the scale of the mesh to the given scale
	void SetScale(glm::vec3 _scale);

	// Returns the size of the mesh if it was converted into a vertex array (needed for allocating memory for ConvertToVertData())
	int GetVertCount();
	// Returns the number of indexes in this mesh
	int GetIndexCount();

	// Adds the given triangle to the mesh
	void AddFace(Face _face);
	// Adds the given vertex as the latest vertex of this mesh
	int AddVert(Vertex _vert);
	// Adds the given vertex to the given index
	int AddVert(int _index, Vertex _vert);
	// Returns the face at the given index
	Face GetFace(size_t index);
	// Returns the vert at the given index
	Vertex GetVert(size_t index);

	// Returns the name of the mesh
	std::string GetName();
	// Sets the name of the mesh
	void SetName(std::string _name);
	// Returns the faces of the mesh
	std::vector<Face>& GetFaces();
	// Returns the tris of the mesh
	std::vector<Triangle>& GetTris(std::vector<Triangle>& _tris);
	// Returns all mats of the mesh for the given vertex
	std::vector<std::string>& GetMatsForVert(std::vector<std::string>& _mats, int vertId);
	// Returns the vertices of the mesh
	std::unordered_map<int, Vertex>& GetVerts();
	// Reorients all tris to have correct direction
	void ReorientTris(std::vector<Triangle>& _tris);

	// Clears the mesh of all faces
	void Clear();

	// Vertex model of the mesh [0 for separate, 1 for indexed]
	int GetVertexModel();

	// Recalculates the normals of the mesh
	void RecalculateNormals();

	OldMesh();
	~OldMesh();
};