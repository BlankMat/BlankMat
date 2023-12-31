#include "selection.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/scene.h"

// Set the texture selecting mode
void Selection::SetIsSelectingTexture(bool isSelecting)
{
	mIsSelectingTexture = isSelecting;
}

// Set the material selecting mode
void Selection::SetIsSelectingMaterial(bool isSelecting)
{
	mIsSelectingMaterial = isSelecting;
}

// Returns whether texture selecting mode is on
bool Selection::IsSelectingTexture()
{
	return mIsSelectingTexture;
}

// Returns whether material selecting mode is on
bool Selection::IsSelectingMaterial()
{
	return mIsSelectingMaterial;
}

// Sets the texture to select
void Selection::SetTextureInSelect(Texture* texture)
{
	mTextureInSelect = texture;
}

// Sets the material to select
void Selection::SetMaterialInSelect(Material* material)
{
	mMaterialInSelect = material;
}

// Returns the texture being selected
Texture* Selection::GetTextureInSelect()
{
	return mTextureInSelect;
}

// Returns the material being selected
Material* Selection::GetMaterialInSelect()
{
	return mMaterialInSelect;
}

// Disables all active selection modes
void Selection::DisableSelectionModes()
{
	mIsSelectingMaterial = false;
	mIsSelectingTexture = false;
}

// Returns the entire selection as a selection of vertices
void Selection::GetSelectedVerts(std::vector<unsigned int>& _verts)
{
	/*
	// Add all faces to selection
	for (auto iter = selFaces.begin(); iter != selFaces.end(); ++iter) {
		Face tempFace = selMesh->GetFace(*iter);
		for (int i = 0; i < tempFace.GetNumVerts(); i++) {
			_verts.push_back(tempFace.vertices[i]);
		}
	}

	// Add all verts to selection
	for (auto iter = selVerts.begin(); iter != selVerts.end(); ++iter) {
		_verts.push_back(*iter);
	}
	*/
}

// Returns the entire selection as a selection of vertices
void Selection::GetSelectedVerts(std::set<unsigned int>& _verts)
{
	/*
	// Add all faces to selection
	for (auto iter = selFaces.begin(); iter != selFaces.end(); ++iter) {
		Face tempFace = selMesh->GetFace(*iter);
		for (int i = 0; i < tempFace.GetNumVerts(); i++) {
			_verts.emplace(tempFace.vertices[i]);
		}
	}

	// Add all verts to selection
	for (auto iter = selVerts.begin(); iter != selVerts.end(); ++iter) {
		_verts.emplace(*iter);
	}
	*/
}

// Selects the face with the given ID
void Selection::SelectFace(unsigned int _id, bool _deselect)
{
	if (_id < 0)
		return;
	if (_deselect)
		mSelFaces.clear();

	mSelFaces.emplace(_id);
	std::cout << "Selected face [" << _id << "].\n";
}

// Selects the vertex with the given ID
void Selection::SelectVert(unsigned int _id, bool _deselect)
{
	if (_id < 0)
		return;
	if (_deselect)
		mSelVertices.clear();

	mSelVertices.emplace(_id);
	std::cout << "Selected vertex [" << _id << "].\n";
}

// Selects the given mesh
void Selection::SelectMesh(Mesh* mesh)
{
	mSelMesh = mesh;
	SelectElement(mesh);
	std::cout << "Selected mesh [" << IEntity::GetNameNullSafe(mSelMesh) << "].\n";
}

// Selects the given entity
void Selection::SelectEntity(IEntity* entity)
{
	mSelEntity = entity;
	SelectElement(entity);
	UpdateTransformHandle();
	std::cout << "Selected entity [" << IEntity::GetNameNullSafe(mSelEntity) << "].\n";
}

// Selects the given material
void Selection::SelectMaterial(Material* material)
{
	mSelMaterial = material;
	SelectElement(material);
	std::cout << "Selected material [" << (material != nullptr ? material->GetScopedName() : "None") << "].\n";
}

// Selects the given selectable element
void Selection::SelectElement(ISelectable* element)
{
	DisableSelectionModes();
	mSelElement = element;
	std::cout << "Selected element [" << (element != nullptr ? (int)element->GetSelectableType() : -1) << "].\n";
}

// Deselects the face with the given ID
void Selection::DeselectFace(unsigned int _id)
{
	mSelFaces.erase(_id);
	std::cout << "Deselected face [" << _id << "].\n";
}

// Deselects the vertex with the given ID
void Selection::DeselectVert(unsigned int _id)
{
	mSelVertices.erase(_id);
	std::cout << "Deselected vertex [" << _id << "].\n";
}

// Deselects the currently selected entity
void Selection::DeselectEntity()
{
	if (mSelEntity == nullptr)
		return;

	mSelEntity = nullptr;
	DisableSelectionModes();
	UpdateTransformHandle();
	std::cout << "Deselected entity.\n";
}

// Deselects the currently selected mesh
void Selection::DeselectMesh()
{
	if (mSelMesh == nullptr)
		return;

	mSelMesh = nullptr;
	DisableSelectionModes();
	std::cout << "Deselected mesh.\n";
}

// Deselects the currently selected material
void Selection::DeselectMat()
{
	if (mSelMaterial == nullptr)
		return;

	mSelMaterial = nullptr;
	DisableSelectionModes();
	std::cout << "Delected material\n";
}

// Deselects the currently selected element
void Selection::DeselectElement()
{
	if (mSelElement == nullptr)
		return;

	mSelElement = nullptr;
	DisableSelectionModes();
	std::cout << "Deselected element\n";
}

// Clears the vertex selection
void Selection::ClearVertSel()
{
	mSelVertices.clear();
}

// Clears the face selection
void Selection::ClearFaceSel()
{
	mSelFaces.clear();
}

// Clears the entire selection
void Selection::ClearSelection()
{
	ClearFaceSel();
	ClearVertSel();
}

// Sets the selection pivot
void Selection::SetSelectionPivot(glm::vec3 _pivot)
{
	mPivot = _pivot;
}

// Returns the selection pivot
glm::vec3 Selection::GetSelectionPivot()
{
	return mPivot;
}

// Calculates the selection pivot
void Selection::CalcSelPivot()
{
	// Reset pivot if nothing is selected
	if (mSelMesh == nullptr)
		mPivot = glm::vec3(0, 0, 0);
	// Use mesh pivot as center
	else if (mSelVertices.size() == 0 && mSelFaces.size() == 0)
		mPivot = mSelMesh->GetPos();
	// Calculate center for face selection
	else if (mSelFaces.size() != 0) {
		mPivot = mSelMesh->GetPos();
	}
	// Calculate center for vertex selection
	else if (mSelVertices.size() != 0) {
		mPivot = glm::vec3(0, 0, 0);
		for (auto iter = mSelVertices.begin(); iter != mSelVertices.end(); ++iter) {
			mPivot += mSelMesh->GetVertex(*iter)->pos;
		}
		mPivot /= (float)mSelVertices.size();
	}
}

// Sets the tool selection
void Selection::SetTool(Tool _sel)
{
	mSelTool = _sel;
	switch (mSelTool) {
	case Tool::NONE:
		std::cout << "Selected tool [NONE]\n";
		break;
	case Tool::MOVE:
		std::cout << "Selected tool [MOVE]\n";
		break;
	case Tool::SCALE:
		std::cout << "Selected tool [SCALE]\n";
		break;
	case Tool::ROTATE:
		std::cout << "Selected tool [ROTATE]\n";
		break;
	case Tool::EXTRUDE:
		std::cout << "Selected tool [EXTRUDE]\n";
		break;
	case Tool::SELECT:
		std::cout << "Selected tool [SELECT]\n";
		break;
	}
}
// Sets the selection mode
void Selection::SetSelMode(SelMode _sel)
{
	mSelMode = _sel;
	switch (mSelMode) {
	case SelMode::MESH:
		std::cout << "Selected mode [MESH]\n";
		break;
	case SelMode::FACE:
		std::cout << "Selected mode [FACE]\n";
		break;
	case SelMode::VERT:
		std::cout << "Selected mode [VERT]\n";
		break;
	}
}

// Returns whether the given vertex is selected
bool Selection::IsVertSelected(unsigned int _id) { return mSelVertices.contains(_id); }
// Returns whether the given face is selected
bool Selection::IsFaceSelected(unsigned int _id) { return mSelFaces.contains(_id); }

// Returns the tool selection
Tool Selection::GetTool() { return mSelTool; }
// Returns the selection mode
SelMode Selection::GetSelMode() { return mSelMode; }
// Returns the selected mesh
Mesh* Selection::GetSelectedMesh() { return mSelMesh; }
// Returns the selected entity
IEntity* Selection::GetSelectedEntity() { return mSelEntity; }
// Returns the selected material
Material* Selection::GetSelectedMaterial() { return mSelMaterial; }
// Returns the transform handle
IEntity* Selection::GetTransformHandle() { return mSelTransformHandle; }
// Returns the currently selected element
ISelectable* Selection::GetSelectedElement() { return mSelElement; }

// Sets the selection's transform handle
void Selection::SetTransformHandle(IEntity* transformHandle) { mSelTransformHandle = transformHandle; }

// Toggles the visibility of the selection
void Selection::ToggleSelectionVisibility()
{
	if (mSelEntity != nullptr)
		mSelEntity->ToggleEnabled();
}

// Updates the transform handle's status
void Selection::UpdateTransformHandle()
{
	if (mSelTransformHandle == nullptr)
		return;

	bool isSelected = (mSelEntity != nullptr);
	mSelTransformHandle->Enable(isSelected);
	if (isSelected)
		mSelTransformHandle->SetParentModelMatrix(mSelEntity->GetModelMatrix());
}

// Returns the nearest mesh to the clicked position
Mesh* Selection::GetNearestMesh(Scene* scene, float u, float v)
{
	//std::cout << "Transformed click [" << u << ", " << v << "]\n";
	//Ray ray = RayTracer::GenerateRay(scene, u, v, false);
	//if (ray.DoesIntersect(scene->GetRenderTris()))
	//	return scene->GetMeshes()->GetAll().begin()->second;
	//else
	//	return nullptr;
	return nullptr;
}

// Returns the nearest vertex to the clicked position
int Selection::GetNearestVert(Scene* scene, float u, float v)
{
	//Ray ray = RayTracer::GenerateRay(scene, u, v, false);
	//IndVertex res = ray.GetClosestVertex(scene->GetRenderTris());
	//return res.id;
	return -1;
}

// Returns the nearest face to the clicked position
int Selection::GetNearestFace(Scene* scene, float u, float v)
{
	//Ray ray = RayTracer::GenerateRay(scene, u, v, false);
	//ITriangle res = ray.GetClosestTriangle(scene->GetRenderTris());
	//return res.triIndex;
	return -1;
}

// Resets the selection
void Selection::Reset()
{
	mSelMesh = nullptr;
	mSelEntity = nullptr;
	mSelMaterial = nullptr;
	mSelElement = nullptr;

	mSelTool = Tool::SELECT;
	mSelMode = SelMode::MESH;
}

// Storage container for information on all selections
Selection::Selection()
{
	Reset();
}