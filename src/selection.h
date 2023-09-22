#pragma once
#include "glIncludes.h"
#include "oldmesh.h"
#include "indexStructs.h"
#include <iostream>
#include <set>

enum class Tool { NONE = 0, SELECT = 1, MOVE = 2, ROTATE = 3, SCALE = 4, EXTRUDE = 5 };
enum class SelMode { MESH = 0, VERT = 1, FACE = 2 };

// Define scene to prevent circular dependency
class Scene;

struct Selection
{
private:
	glm::vec3 pivot;
	std::set<int> selVerts;
	std::set<int> selFaces;
	OldMesh* selMesh;

	Tool tool;
	SelMode selMode;
public:
	std::set<int> newSelVerts;
	std::set<int> removedSelVerts;

	// Returns the entire selection as a selection of vertices
	void GetSelectedVerts(std::vector<int>& _verts);
	// Returns the entire selection as a selection of vertices
	void GetSelectedVerts(std::set<int>& _verts);

	// Selects the face with the given ID
	void SelectFace(int _id, bool _deselect = false);
	// Selects the vertex with the given ID
	void SelectVert(int _id, bool _deselect = false);
	// Selects the given mesh
	void SelectMesh(OldMesh* mesh);
	// Deselects the face with the given ID
	void DeselectFace(int _id);
	// Deselects the vertex with the given ID
	void DeselectVert(int _id);
	// Deselects the currently selected mesh
	void DeselectMesh();

	// Clears the vertex selection
	void ClearVertSel();
	// Clears the face selection
	void ClearFaceSel();
	// Clears the entire selection
	void ClearSelection();
	// Sets the selection pivot
	void SetSelectionPivot(glm::vec3 _pivot);
	// Returns the selection pivot
	glm::vec3 GetSelectionPivot();
	// Calculates the selection pivot
	void CalcSelPivot();
	// Sets the tool selection
	void SetTool(Tool _sel);
	// Sets the selection mode
	void SetSelMode(SelMode _sel);
	// Returns the tool selection
	Tool GetTool();
	// Returns the selection mode
	SelMode GetSelMode();
	// Returns the selected mesh
	OldMesh* GetSelectedMesh();
	// Returns whether the given vertex is selected
	bool IsVertSelected(int _id);
	// Returns whether the given face is selected
	bool IsFaceSelected(int _id);

	// Storage container for information on all selections
	Selection();

	// Returns the nearest mesh to the clicked position
	static OldMesh* GetNearestMesh(Scene* scene, int i, int j);
	// Returns the nearest vertex to the clicked position
	static int GetNearestVert(Scene* scene, int i, int j);
	// Returns the nearest face to the clicked position
	static int GetNearestFace(Scene* scene, int i, int j);
};