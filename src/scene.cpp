#include "scene.h"

void Scene::Draw(Window* window)
{
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();
	if (curShader == "")
		UseShader("default");
	if (model != nullptr)
		model->Draw(*shaders[curShader], viewProj);
	if (light != nullptr)
		light->Draw(viewProj);
}

void Scene::UseShader(std::string name)
{
	if (shaders.find(name) != shaders.end() && shaders[name] != nullptr)
	{
		shaders[name]->Use();
		curShader = name;
	}
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

Scene::Scene()
{
	camera = nullptr;
	light = nullptr;
	model = nullptr;
	shaders = std::unordered_map<std::string, Shader*>();
}

Scene::~Scene()
{
	if (camera != nullptr)
		delete camera;
	if (light != nullptr)
		delete light;
	if (model != nullptr)
		delete model;

	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		delete iter->second;
	shaders.clear();
}