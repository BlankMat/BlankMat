#pragma once
#include "glIncludes.h"
#include "interfaces/iSelectable.h"
#include <vector>
#include <iostream>
#include <set>

enum class Tool { NONE = 0, SELECT = 1, MOVE = 2, ROTATE = 3, SCALE = 4, EXTRUDE = 5 , LAST};
enum class SelMode { MESH = 0, VERT = 1, FACE = 2, LAST };

// Forward declare to prevent circular dependency
class IEntity;
class Mesh;
class Scene;
class Material;

class Selection
{
private:
	glm::vec3 mPivot = glm::vec3(0.0f);
	std::set<unsigned int> mSelVertices;
	std::set<unsigned int> mSelFaces;
	Mesh* mSelMesh = nullptr;
	IEntity* mSelEntity = nullptr;
	Material* mSelMaterial = nullptr;
	IEntity* mSelTransformHandle = nullptr;
	ISelectable* mSelElement = nullptr;

	Tool mSelTool = Tool::SELECT;
	SelMode mSelMode = SelMode::MESH;

public:
	std::set<unsigned int> newSelVerts;
	std::set<unsigned int> removedSelVerts;

	// Returns the entire selection as a selection of vertices
	void GetSelectedVerts(std::vector<unsigned int>& _verts);
	// Returns the entire selection as a selection of vertices
	void GetSelectedVerts(std::set<unsigned int>& _verts);

	// Selects the face with the given ID
	void SelectFace(unsigned int _id, bool _deselect = false);
	// Selects the vertex with the given ID
	void SelectVert(unsigned int _id, bool _deselect = false);
	// Selects the given mesh
	void SelectMesh(Mesh* mesh);
	// Selects the given entity
	void SelectEntity(IEntity* entity);
	// Selects the given material
	void SelectMaterial(Material* material);
	// Selects the given selectable element
	void SelectElement(ISelectable* element);
	// Deselects the face with the given ID
	void DeselectFace(unsigned int _id);
	// Deselects the vertex with the given ID
	void DeselectVert(unsigned int _id);
	// Deselects the currently selected entity
	void DeselectEntity();
	// Deselects the currently selected mesh
	void DeselectMesh();
	// Deselects the currently selected material
	void DeselectMat();
	// Deselects the currently selected element
	void DeselectElement();

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
	Mesh* GetSelectedMesh();
	// Returns the selected entity
	IEntity* GetSelectedEntity();
	// Returns the selected material
	Material* GetSelectedMaterial();
	// Returns the transform handle
	IEntity* GetTransformHandle();
	// Returns the currently selected element
	ISelectable* GetSelectedElement();
	// Returns whether the given vertex is selected
	bool IsVertSelected(unsigned int _id);
	// Returns whether the given face is selected
	bool IsFaceSelected(unsigned int _id);
	
	// Toggles the visibility of the selection
	void ToggleSelectionVisibility();

	// Sets the selection's transform handle
	void SetTransformHandle(IEntity* transformHandle);
	// Updates the transform handle's status
	void UpdateTransformHandle();

	// Resets the selection
	void Reset();

	/// <summary>
	/// Returns the nearest mesh to the clicked position
	/// </summary>
	/// <param name="scene">Scene to check</param>
	/// <param name="u">U coordinate onscreen</param>
	/// <param name="v">V coordinate onscreen</param>
	/// <returns>Selected mesh</returns>
	static Mesh* GetNearestMesh(Scene* scene, float u, float v);

	/// <summary>
	/// Returns the nearest vertex to the clicked position
	/// </summary>
	/// <param name="scene">Scene to check</param>
	/// <param name="u">U coordinate onscreen</param>
	/// <param name="v">V coordinate onscreen</param>
	/// <returns>Selected vertex index, or -1 if none was found</returns>
	static int GetNearestVert(Scene* scene, float u, float v);

	/// <summary>
	/// Returns the nearest face to the clicked position
	/// </summary>
	/// <param name="scene">Scene to check</param>
	/// <param name="u">U coordinate onscreen</param>
	/// <param name="v">V coordinate onscreen</param>
	/// <returns>Selected face index, or -1 if none was found</returns>
	static int GetNearestFace(Scene* scene, float u, float v);

	/// <summary>
	/// Storage container for information on all selections
	/// </summary>
	Selection();
};