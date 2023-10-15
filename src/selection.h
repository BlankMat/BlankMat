#pragma once
#include "glIncludes.h"
#include <vector>
#include <iostream>
#include <set>

enum class Tool { NONE = 0, SELECT = 1, MOVE = 2, ROTATE = 3, SCALE = 4, EXTRUDE = 5 };
enum class SelMode { MESH = 0, VERT = 1, FACE = 2 };

// Forward declare to prevent circular dependency
class IEntity;
class IMesh;
class IScene;

struct Selection
{
private:
	glm::vec3 mPivot;
	std::set<unsigned int> mSelVertices;
	std::set<unsigned int> mSelFaces;
	IMesh* mSelMesh;
	IEntity* mSelEntity;

	Tool mSelTool;
	SelMode mSelMode;
public:
	std::set<unsigned int> newSelVerts;
	std::set<unsigned int> removedSelVerts;

	/// <summary>
	/// Storage container for information on all selections
	/// </summary>
	Selection();

	// Returns the entire selection as a selection of vertices
	void GetSelectedVerts(std::vector<unsigned int>& _verts);
	// Returns the entire selection as a selection of vertices
	void GetSelectedVerts(std::set<unsigned int>& _verts);

	// Selects the face with the given ID
	void SelectFace(unsigned int _id, bool _deselect = false);
	// Selects the vertex with the given ID
	void SelectVert(unsigned int _id, bool _deselect = false);
	// Selects the given mesh
	void SelectMesh(IMesh* mesh);
	// Selects the given entity
	void SelectEntity(IEntity* entity);
	// Deselects the face with the given ID
	void DeselectFace(unsigned int _id);
	// Deselects the vertex with the given ID
	void DeselectVert(unsigned int _id);
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
	IMesh* GetSelectedMesh();
	// Returns the selected entity
	IEntity* GetSelectedEntity();
	// Returns whether the given vertex is selected
	bool IsVertSelected(unsigned int _id);
	// Returns whether the given face is selected
	bool IsFaceSelected(unsigned int _id);

	/// <summary>
	/// Returns the nearest mesh to the clicked position
	/// </summary>
	/// <param name="scene">Scene to check</param>
	/// <param name="u">U coordinate onscreen</param>
	/// <param name="v">V coordinate onscreen</param>
	/// <returns>Selected mesh</returns>
	static IMesh* GetNearestMesh(IScene* scene, float u, float v);

	/// <summary>
	/// Returns the nearest vertex to the clicked position
	/// </summary>
	/// <param name="scene">Scene to check</param>
	/// <param name="u">U coordinate onscreen</param>
	/// <param name="v">V coordinate onscreen</param>
	/// <returns>Selected vertex index, or -1 if none was found</returns>
	static int GetNearestVert(IScene* scene, float u, float v);

	/// <summary>
	/// Returns the nearest face to the clicked position
	/// </summary>
	/// <param name="scene">Scene to check</param>
	/// <param name="u">U coordinate onscreen</param>
	/// <param name="v">V coordinate onscreen</param>
	/// <returns>Selected face index, or -1 if none was found</returns>
	static int GetNearestFace(IScene* scene, float u, float v);
};