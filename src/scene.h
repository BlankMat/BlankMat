#pragma once
#include "glIncludes.h"
#include "shader.h"
#include "vertex.h"
#include "window.h"
#include "grid.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "selection.h"
#include <unordered_map>

class Scene
{
private:
	std::string curShader;
	Light* light;
	Camera* camera;
	Model* model;
	std::vector<Drawable*> drawables;
	std::unordered_map<std::string, Shader*> shaders;
public:
	// Renders the current scene
	void Draw(Window* window);
	// Activates the shader with the given name for the scene
	void UseShader(std::string name);
	// Creates a shader for the scene with the given name from the source file of the given name
	void CreateShader(std::string name, bool loadGeom);
	// Creates a shader for the scene with the given name, loading it from a different source than the name
	void CreateShader(std::string name, std::string source, bool loadGeom);

	// Returns the scene's camera
	Camera* GetCamera() { return camera; }
	// Returns the scene's light
	Light* GetLight() { return light; }
	// Returns the scene's model
	Model* GetModel() { return model; }
	// Returns the shader with the given name
	Shader* GetShader(std::string name) { return shaders[name]; }

	// Sets up the scene's camera with the given options
	void SetCamera(Options* options) { if (camera != nullptr) { delete camera; } camera = new Camera(options); }
	// Sets the scene's camera to the given camera
	void SetCamera(Camera* _cam) { if (camera != nullptr) { delete camera; } camera = _cam; }
	// Sets the scene's light to the given light
	void SetLight(Light* _light) { if (light != nullptr) { delete light; } light = _light; }
	// Sets the scene's model to the given model
	void SetModel(Model* _model) { if (model != nullptr) { delete model; } model = _model; }
	// Adds a drawable to the scene's render list
	void AddDrawable(Drawable* _drawable) { drawables.push_back(_drawable); }

	// Returns the projection matrix of the scene's camera
	glm::mat4 GetProjectionMatrix(float aspect) { return GetCamera()->GetProjection(aspect); }
	// Returns the view matrix of the scene's camera
	glm::mat4 GetViewMatrix() { return GetCamera()->GetView(); }

	// Constructs the scene, getting everything ready for manual setting
	Scene();
	// Destructs scene and cleans up all memory used
	~Scene();
};
