#include "scene.h"
#include "openGLHelper.h"

void Scene::Draw(std::string name)
{
	if (shaders.find(name) != shaders.end() && shaders[name] != nullptr)
		shaders[name]->use();
}

void Scene::CreateShader(std::string name, bool loadGeom)
{
	if (shaders.find(name) == shaders.end())
		shaders.emplace(name, new Shader(name, loadGeom));
}

void Scene::CreateShader(std::string name, std::string source, bool loadGeom)
{
	if (shaders.find(name) == shaders.end())
		shaders.emplace(name, new Shader(source, loadGeom));
}

void Scene::GetVAO(float* vertices, int vertsSize, unsigned int* indices, int indicesSize, Selection* _sel)
{
	//std::cout << "Writing " << vertsSize << " vertices to VAO, " << verts.size() << " exist" << std::endl;
	//std::cout << "Writing " << indicesSize << " indices to VAO, " << tris.size() << " exist" << std::endl;

	int tempHighIndex = -1;
	int startIndex = 0;
	std::set<int> selectedVerts;
	if (_sel != nullptr)
		_sel->GetSelectedVerts(selectedVerts);

	for (auto iter = GetMeshes()->GetAll().begin(); iter != GetMeshes()->GetAll().end(); ++iter) {
		OldMesh* curMesh = iter->second;
		std::unordered_map<int, Vertex> verts = curMesh->GetVerts();
		glm::vec3 pos = curMesh->GetPos();
		// Track out indices separate from loop
		for (auto viter = verts.begin(); viter != verts.end(); ++viter) {
			int i = viter->first;
			glm::vec3 vertPos = verts[i].pos;
			glm::vec3 vertNorm = verts[i].normal;
			glm::vec2 vertTexCoord = verts[i].texCoords;

			// Get all mats for vertex
			std::vector<std::string> _matKeys;
			std::vector<Material*> _mats;
			curMesh->GetMatsForVert(_matKeys, i);
			for (int j = 0; j < _matKeys.size(); j++) {
				_mats.push_back(mats->Get(_matKeys[j]));
			}
			Material vertMat = Material::Average(_mats);

			// Selection
			vertices[startIndex + VERT_SHADER_SIZE * i + 0] = GetVertSelection(selectedVerts, i);
			// Position
			// For every vertex, 10 elements are stored, so the array index is 10*i + 1 + <elem pos>
			vertices[startIndex + VERT_SHADER_SIZE * i + 1] = vertPos.x;
			vertices[startIndex + VERT_SHADER_SIZE * i + 2] = vertPos.y;
			vertices[startIndex + VERT_SHADER_SIZE * i + 3] = vertPos.z;

			// Normal
			vertices[startIndex + VERT_SHADER_SIZE * i + 4] = vertNorm.x;
			vertices[startIndex + VERT_SHADER_SIZE * i + 5] = vertNorm.y;
			vertices[startIndex + VERT_SHADER_SIZE * i + 6] = vertNorm.z;

			// Color
			vertices[startIndex + VERT_SHADER_SIZE * i + 7] = glm::clamp(vertMat.kd.r + vertMat.ka.r, 0.0f, 1.0f);
			vertices[startIndex + VERT_SHADER_SIZE * i + 8] = glm::clamp(vertMat.kd.g + vertMat.ka.g, 0.0f, 1.0f);
			vertices[startIndex + VERT_SHADER_SIZE * i + 9] = glm::clamp(vertMat.kd.b + vertMat.ka.b, 0.0f, 1.0f);

			// TexCoord
			vertices[startIndex + VERT_SHADER_SIZE * i + 10] = vertTexCoord.x;
			vertices[startIndex + VERT_SHADER_SIZE * i + 11] = vertTexCoord.y;

			tempHighIndex = VERT_SHADER_SIZE * i + (VERT_SHADER_SIZE - 1);
		}
		startIndex += (int)verts.size();
	}
	//std::cout << "Highest element written to vertices: " << tempHighIndex << std::endl;

	// Track out indices separate from loop
	std::vector<Triangle> tris = GetTris();
	for (int i = 0; i < tris.size(); i++) {
		for (int j = 0; j < TRI_VERTS; j++) {
			indices[i * TRI_VERTS + j] = tris[i].vertices[j];
			tempHighIndex = i * TRI_VERTS + j;
		}
	}
	//std::cout << "Highest element written to indices: " << tempHighIndex << std::endl;
}

void Scene::CalcRenderTris()
{
	// Clear previous tris
	renderTris.clear();

	// Calculate new tris
	int mi = 0;
	for (auto iter = meshes->GetAll().begin(); iter != meshes->GetAll().end(); ++iter) {
		OldMesh* tempMesh = meshes->Get(iter->first);
		std::vector<Triangle> tempTris;
		tempMesh->GetTris(tempTris);

		// Convert all tris to info tris
		for (int i = 0; i < tempTris.size(); i++) {
			renderTris.push_back(ITriangle(tempTris[i], tempMesh->GetVerts(), tempTris[i].triIndex, mi, tempTris[i].tag));
		}
		mi++;
	}

	// Ensure all tris are facing the correct way
	for (int i = 0; i < renderTris.size(); i++) {
		renderTris[i].Reorient(renderTris);
	}

	// Make tris out of render tris
	tris.clear();
	for (int i = 0; i < renderTris.size(); i++) {
		tris.push_back(Triangle(
			renderTris[i].vertices[0].id,
			renderTris[i].vertices[1].id,
			renderTris[i].vertices[2].id,
			renderTris[i].normal, renderTris[i].center,
			renderTris[i].mat, renderTris[i].shadingGroup, renderTris[i].triIndex, renderTris[i].tag));
	}
}

float Scene::GetVertSelection(std::set<int>& verts, int i)
{
	if (verts.size() != 0)
		return (verts.find(i) != verts.end() ? 1.0f : 0.0f);
	else
		return 0.0f;
}

std::unordered_map<int, Vertex>& Scene::GetVerts()
{
	return verts;
}

std::vector<Triangle>& Scene::GetTris()
{
	return tris;
}

std::vector<ITriangle>& Scene::GetRenderTris()
{
	return renderTris;
}

int Scene::GetVertCount()
{
	int count = 0;
	for (auto iter = meshes->GetAll().begin(); iter != meshes->GetAll().end(); ++iter) {
		count += (int)meshes->Get(iter->first)->GetVerts().size();
	}
	return count;
}

int Scene::GetIndexCount()
{
	int count = 0;
	for (auto iter = meshes->GetAll().begin(); iter != meshes->GetAll().end(); ++iter) {
		for (int i = 0; i < meshes->Get(iter->first)->GetFaces().size(); i++) {
			// ngon will create (n-2) triangles with 3 vertices each
			count += (meshes->Get(iter->first)->GetFaces()[i].GetNumVerts() - 2) * 3;
		}
	}
	return count;
}

Camera* Scene::GetCamera() { return camera; }
Light* Scene::GetLight() { return light; }
MaterialStorage* Scene::GetMats() { return mats; }
MeshStorage* Scene::GetMeshes() { return meshes; }
Shader* Scene::GetShader(std::string name) { return shaders[name]; }

void Scene::SetCameraFromOptions(Options* options) { camera->SetFromOptions(options); }
void Scene::SetCamera(Camera* _cam)
{
	if (camera != nullptr)
		delete camera;
	camera = _cam;
}

void Scene::SetLight(Light* _light)
{
	if (light != nullptr)
		delete light;
	light = _light;
}

void Scene::SetMats(MaterialStorage* _mats)
{
	if (mats != nullptr)
		delete mats;
	mats = _mats;
}

void Scene::SetMeshes(MeshStorage* _meshes)
{
	if (meshes != nullptr)
		delete meshes;
	meshes = _meshes;
}

Scene::Scene()
{
	camera = new Camera();
	light = new Light();
	mats = new MaterialStorage();
	meshes = new MeshStorage();
	shaders = std::unordered_map<std::string, Shader*>();
	invMVP = glm::mat4();

	meshes->AddMesh("defaultMesh", new OldMesh());
}

Scene::~Scene()
{
	delete camera;
	delete light;
	delete mats;
	delete meshes;

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		delete iter->second;
	shaders.clear();
	renderTris.clear();
	tris.clear();
	verts.clear();
}

// Calculates the model view perspective matrix
// --------------------------------------------
glm::mat4 Scene::CalcMVP()
{
	return GetProjectionMatrix() * GetViewMatrix() * GetModelMatrix();
}

// Calculates the inverse of the model view persepctive matrix
glm::mat4 Scene::CalcInvMVP()
{
	return glm::inverse(GetProjectionMatrix() * GetViewMatrix()) * GetModelMatrix();
}

// Returns the projection matrix of the given camera
glm::mat4 Scene::GetProjectionMatrix()
{
	OldMesh* mesh = GetMeshes()->GetAll().begin()->second;
	Camera* camera = GetCamera();

	// Projection
	glm::mat4 projection = glm::mat4(1.0f);

	// Perspective projection
	if ((*camera).isPerspective) {
		projection = glm::perspective(glm::radians((*camera).fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, (*camera).nearClip, (*camera).farClip);
	}
	// Orthographic projection
	else {
		glm::mat4 projection = glm::ortho(-(*camera).orthSize.x, (*camera).orthSize.x, -(*camera).orthSize.y, (*camera).orthSize.y,
			(*camera).nearClip, (*camera).farClip);
	}

	return projection;
}

// Returns the view matrix of the given camera
glm::mat4 Scene::GetViewMatrix()
{
	Camera* camera = GetCamera();

	// Camera view
	return glm::lookAt((*camera).pos, (*camera).pos + (*camera).dir, (*camera).up);
}

// Returns the model matrix of the given mesh
glm::mat4 Scene::GetModelMatrix()
{
	OldMesh* mesh = GetMeshes()->GetAll().begin()->second;

	// Model position
	glm::vec3 scale = mesh->GetScale();
	glm::vec3 rotation = mesh->GetRotation();
	glm::vec3 position = mesh->GetPos();
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(0, 1, 0));
	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(1, 0, 0));
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));

	glm::mat4 modelRotated = rotateZ * rotateY * rotateX * translate;
	glm::mat4 modelUnscaled = glm::translate(modelRotated, position);
	return glm::scale(modelUnscaled, scale);
}
