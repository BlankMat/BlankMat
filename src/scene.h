#pragma once
#include "glIncludes.h"
#include "light.h"
#include "camera.h"
#include "materialStorage.h"
#include "meshStorage.h"
#include "itriangle.h"
#include "ray.h"
#include "selection.h"
#include "vertex.h"
#include "shader.h"

class Scene
{
private:
	Light* light;
	Camera* camera;
	MaterialStorage* mats;
	MeshStorage* meshes;
	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<int, Vertex> verts;
	std::vector<Triangle> tris;
	std::vector<ITriangle> renderTris;
	glm::mat4 invMVP;
public:
	glm::vec3 bgColor = glm::vec3(200, 200, 200);

	void Draw(std::string name);
	void CreateShader(std::string name, bool loadGeom);
	void CreateShader(std::string name, std::string source, bool loadGeom);

	void GetVAO(float* vertices, int vertsSize, unsigned int* indices, int indicesSize, Selection* _sel = nullptr);
	void CalcRenderTris();
	float GetVertSelection(std::set<int>& verts, int i);

	std::unordered_map<int, Vertex>& GetVerts();
	std::vector<Triangle>& GetTris();
	std::vector<ITriangle>& GetRenderTris();
	int GetVertCount();
	int GetIndexCount();

	Camera* GetCamera();
	Light* GetLight();
	MaterialStorage* GetMats();
	MeshStorage* GetMeshes();
	Shader* GetShader(std::string);

	void SetCameraFromOptions(Options* options);
	void SetCamera(Camera* _cam);
	void SetLight(Light* _light);
	void SetMats(MaterialStorage* _mats);
	void SetMeshes(MeshStorage* _meshes);

	glm::mat4 CalcMVP();
	glm::mat4 CalcInvMVP();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetModelMatrix();

	Scene();
	~Scene();
};
