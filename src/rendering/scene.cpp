#include "rendering/scene.h"
#include "windows/window.h"

// Renders the current scene
void Scene::Draw(Window* window, Shader* shader)
{
	// Calculate camera view projection
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();

	// Draw flat objects
	Shader* flatShader = GetShader("flat");
	if (flatShader != nullptr)
	{
		flatShader->Use();
		if (mGrid != nullptr && mState->enableGrid)
			mGrid->Draw(flatShader, mState, mDefaultMat, viewProj, true);

		if (mGlobalLight != nullptr)
			mGlobalLight->Draw(flatShader, mState, mDefaultMat, viewProj, true);
	}

	// Draw all of the scene
	shader->Use();
	GetLight()->UpdateShader(shader);
	shader->SetVec3("viewPos", GetCamera()->GetPos());

	// Draw scene by materials
	if (mState->drawByMaterial)
	{
		for (auto iter = mMeshRenderList.begin(); iter != mMeshRenderList.end(); ++iter)
		{
			// If there are meshes to render for the material, update the shader
			Material* mat = GetMaterial(iter->first);
			if (iter->second->Empty())
				continue;

			// If there is a material, preload it
			if (mat != nullptr)
			{
				// Load textures
				mat->LoadTextures(mState, mDefaultMat);
				unsigned int shadowIndex = mat->UpdateShader(shader);
				glActiveTexture(GL_TEXTURE0 + shadowIndex);
				glBindTexture(GL_TEXTURE_2D, mState->depthMap);
				shader->SetInt("shadowMap", mState->depthMapFBO);
				glActiveTexture(GL_TEXTURE0);

				// Draw mesh
				iter->second->Draw(shader, mState, mDefaultMat, viewProj, false);
			}
			// If no material is defined, render the object with its own default mat
			else
			{
				iter->second->Draw(shader, mState, mDefaultMat, viewProj, true);
			}
		}
	}
	// Default draw
	else
	{
		mRootNode->Draw(shader, mState, mDefaultMat, viewProj, true);
	}

	// Draw all entities with their respective shaders
	for (auto iter = mEntityList.begin(); iter != mEntityList.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			Shader* curShader = GetShader(iter->first);
			if (curShader != nullptr)
			{
				curShader->Use();
				curShader->SetVec3("viewPos", GetCamera()->GetPos());
				GetLight()->UpdateShader(curShader);
				iter->second->Draw(curShader, mState, mDefaultMat, viewProj, true);
			}
		}
	}

	// Draw flat objects on top
	if (flatShader != nullptr)
	{
		flatShader->Use();
		if (mViewAxisHandle != nullptr)
		{
			mViewAxisHandle->SetRot(GetRotationDegrees(GetCamera()->GetDir()));
			mViewAxisHandle->Draw(flatShader, mState, mDefaultMat, GetViewAxisProjection(window), true);
		}

		if (mTransformHandle != nullptr)
		{
			mState->GetSel()->UpdateTransformHandle();
			mTransformHandle->Draw(flatShader, mState, mDefaultMat, viewProj, true);
		}
	}
}

// Renders the current scene's shadows
void Scene::DrawShadows(Window* window, Shader* shader)
{
	// Calculate camera view projection
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();

	// Draw all of the scene
	GetLight()->UpdateShader(shader);
	shader->SetVec3("viewPos", GetCamera()->GetPos());
	mRootNode->DrawShadows(shader, mState);
}

// Returns the projection matrix of the view axis handle
const glm::mat4& Scene::GetViewAxisProjection(Window* window)
{
	if (window->GetWidth() != mViewAxisPrevWidth || window->GetHeight() != mViewAxisPrevHeight)
	{
		mViewAxisPrevWidth = window->GetWidth();
		mViewAxisPrevHeight = window->GetHeight();
		mViewAxisProjection = glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -100.0f, 100.0f);
	}
	return mViewAxisProjection;
}

// Focuses the camera on the currently selected object (if any)
void Scene::FocusCamera()
{
	IEntity* selEntity = mState->GetSel()->GetSelectedEntity();
	GetCamera()->LookAt((selEntity != nullptr) ? selEntity->GetPos() : glm::vec3(0.0f));
}

// Toggles the camera between perspective and orthographic
void Scene::TogglePerspective()
{
	GetCamera()->SetPerspective(!GetCamera()->IsPerspective());
}

// Toggles the rotation mode of the camera
void Scene::ToggleCameraRotationMode()
{
	GetCamera()->SetPivotRotationMode(!GetCamera()->IsRotatingAroundPivot());
}

// Loads the material of the given config. Must be bottom-level config
Material* Scene::LoadMaterial(Config* config, const std::string& name)
{
	// Don't handle null configs
	if (config == nullptr)
		return nullptr;

	// If the material exists already, return it
	Material* material = GetMaterial(name);
	if (material != nullptr)
		return material;

	// Create textures
	Texture* kd = LoadTexture("texture_diffuse", config->GetString("map_kd"), "default_diffuse");
	Texture* ka = LoadTexture("texture_ambient", config->GetString("map_ka"), "default_ambient");
	Texture* ks = LoadTexture("texture_specular", config->GetString("map_ks"), "default_specular");
	Texture* normal = LoadTexture("texture_normal", config->GetString("map_bump"), "default_normal");
	Texture* ns = LoadTexture("texture_height", config->GetString("map_ns"), "default_height");
	Texture* d = LoadTexture("texture_alpha", config->GetString("map_d"), "default_alpha");

	// Create material
	material = new Material(name, config, kd, ka, ks, normal, ns, d);
	AddMaterial(name, material);
	return material;
}

// Loads the given texture or returns the existing one
Texture* Scene::LoadTexture(const std::string& type, const std::string& path, const std::string& defaultName)
{
	// If no path is given, load the default texture
	std::string pathToLoad = path;
	if (path == "")
		pathToLoad = defaultName;

	// Get the name of the texture
	std::string name = pathToLoad.substr(0, pathToLoad.find_last_of('.'));
	if (name == "default")
		name = defaultName;

	// If the texture is already loaded with the same type, return it
	Texture* texture = GetTexture(name);
	if (texture != nullptr && texture->type == type)
		return texture;

	// If the texture is not loaded, load the texture and store it
	texture = new Texture(type, FileSystem::GetPath(TEXTURE_DIR), pathToLoad, name);
	AddTexture(name, texture);
	return texture;
}

// Activates the shader with the given name for the scene
void Scene::UseShader(const std::string& name)
{
	if (mShaderList.find(name) != mShaderList.end() && mShaderList[name] != nullptr)
	{
		mCurShader = name;
		mShader = mShaderList[name];
		mShader->Use();
	}
}

// Creates a shader for the scene with the given name from the source file of the given name
Shader* Scene::CreateShader(const std::string& name, bool loadGeom)
{
	if (mShaderList.find(name) == mShaderList.end())
	{
		Shader* newShader = new Shader(name, loadGeom);
		newShader->name = name;
		mShaderList.emplace(name, newShader);
		return newShader;
	}
	return mShaderList[name];
}

// Creates a shader for the scene with the given name, loading it from a different source than the name
Shader* Scene::CreateShader(const std::string& name, const std::string& source, bool loadGeom)
{
	if (mShaderList.find(name) == mShaderList.end())
	{
		Shader* newShader = new Shader(source, loadGeom);
		newShader->name = name;
		mShaderList.emplace(name, newShader);
		return newShader;
	}
	return mShaderList[name];
}

// Creates a shader for the scene with the given name, loading it from a config
Shader* Scene::CreateShader(const std::string& name, Config* config)
{
	if (mShaderList.find(name) == mShaderList.end())
	{
		Shader* newShader = new Shader(config->GetString("file"), config->GetBool("hasGeomShader"));
		newShader->name = name;
		mShaderList.emplace(name, newShader);
		return newShader;
	}
	return mShaderList[name];
}

// Returns the scene's camera
Camera* Scene::GetCamera() { return mMainCamera; }

// Returns the scene's light
Light* Scene::GetLight() { return mGlobalLight; }

// Returns the scene's name
std::string Scene::GetName() { return mName; }

// Returns the scene's directory
std::string Scene::GetDirectory() { return mDirectory; }

// Returns the shader with the given name
Shader* Scene::GetShader(const std::string& name)
{
	if (mShaderList.find(name) != mShaderList.end())
		return mShaderList[name];
	return nullptr;
}

// Returns the material with the given name
Material* Scene::GetMaterial(const std::string& name)
{
	if (mMaterialList.find(name) != mMaterialList.end())
		return mMaterialList[name];
	return nullptr;
}

// Returns the default material
Material* Scene::GetDefaultMat()
{
	return mDefaultMat;
}

// Returns the material with the given name
Texture* Scene::GetTexture(const std::string& name)
{
	if (mTextureList.find(name) != mTextureList.end())
		return mTextureList[name];
	return nullptr;
}

// Returns the entity with the given name
IEntity* Scene::GetEntity(const std::string& name)
{
	if (mPreRenderList.find(name) != mPreRenderList.end())
		return mPreRenderList[name];
	else if (mRenderList.find(name) != mRenderList.end())
		return mRenderList[name];
	return nullptr;
}

// Returns the transform handle
IEntity* Scene::GetTransformHandle() { return mTransformHandle; }

// Adds the given node to the scene
void Scene::AddNode(Node* node)
{
	mRootNode->AddChild(node);
}

// Adds the given mesh to the scene
void Scene::AddMesh(Mesh* mesh, Node* parent)
{
	if (parent == nullptr)
		parent = mRootNode;
	parent->AddMesh(mesh);
	mMeshList.push_back(mesh);
	SetEntityMaterial(mesh, mesh->GetMaterial());
}

// Rotates the current camera by the given delta degrees
void Scene::RotateCamera(const glm::vec3& delta)
{
	GetCamera()->Rotate(delta);
}

// Translates the current camera by the given delta
void Scene::TranslateCamera(const glm::vec3& delta)
{
	GetCamera()->Translate(delta);
}

// Returns the root node
Node* Scene::GetRootNode()
{
	return mRootNode;
}

// Sets the root node
void Scene::SetRootNode(Node* rootNode)
{
	mRootNode = rootNode;
}

// Returns a reference to the shader list
const std::unordered_map<std::string, Shader*>& Scene::GetShaderList() { return mShaderList; }

// Returns a reference to the material list
const std::unordered_map<std::string, Material*>& Scene::GetMaterialList() { return mMaterialList; }

// Returns a reference to the texture list
const std::unordered_map<std::string, Texture*>& Scene::GetTextureList() { return mTextureList; }

// Returns the current shader
const std::string Scene::GetCurShader() { return mCurShader; }

// Sets up the scene's camera with the given options
void Scene::SetCamera(ActionStack* actionStack, Config* config) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = new Camera(actionStack, config); }

// Sets the scene's camera to the given camera
void Scene::SetCamera(Camera* cam) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = cam; }

// Sets the scene's light to the given light
void Scene::SetLight(Light* light) { if (mGlobalLight != nullptr) { delete mGlobalLight; } mGlobalLight = light; }

// Sets the scene's name
void Scene::SetName(const std::string& name) { mName = name; }

// Sets the scene's directory
void Scene::SetDirectory(const std::string& dir) { mDirectory = dir; }

// Adds an entity to the scene's render list
IEntity* Scene::AddEntity(const std::string& shaderName, IEntity* entity, bool preRender)
{
	if (mEntityList.find(shaderName) == mEntityList.begin())
		mEntityList.emplace(shaderName, new EntityContainer());
	EntityContainer* entityList = mEntityList[shaderName];
	entityList->Add(entity->GetName(), entity);
	return entity;
}

// Adds a texture to the scene's texture list
Texture* Scene::AddTexture(const std::string& name, Texture* texture)
{
	if (mTextureList.find(name) == mTextureList.end())
		mTextureList.emplace(name, texture);
	return texture;
}

// Adds a material to the scene's material list
Material* Scene::AddMaterial(const std::string& name, Material* material)
{
	if (mMaterialList.find(name) == mMaterialList.end())
		mMaterialList.emplace(name, material);
	return material;
}

// Sets the default material of the scene from the material list if the material exists.
Material* Scene::SetDefaultMaterial(const std::string& name)
{
	if (mMaterialList.find(name) != mMaterialList.end())
		mDefaultMat = mMaterialList[name];
	return mDefaultMat;
}

// Sets the default material of the scene. Note: Does not add it to the material list.
Material* Scene::SetDefaultMaterial(Material* material)
{
	mDefaultMat = material;
	return mDefaultMat;
}

// Sets the material of the given entity
void Scene::SetEntityMaterial(IEntity* entity, Material* material)
{
	std::string prevMatName = entity->GetMaterial()->name;
	std::string matName = material->name;

	// If the material is already stored in the render list, remove it
	if (mMeshRenderList.find(prevMatName) != mMeshRenderList.end())
	{
		if (mMeshRenderList[prevMatName]->Get(entity->GetName()) != nullptr)
			mMeshRenderList[prevMatName]->Remove(entity->GetName());
	}
	entity->SetMaterial(material);

	// Add the material to the appropriate render list
	if (mMeshRenderList.find(material->name) == mMeshRenderList.end())
	{
		mMeshRenderList.emplace(material->name, new EntityContainer());
	}
	mMeshRenderList[material->name]->Add(entity->GetName(), entity);
}

// Loads the materials from the given config
void Scene::LoadMaterials(Config* config)
{
	// Load the default material if it exists
	LoadMaterial(config->GetConfig("default"), "default");

	// Load the rest of the materials
	const std::unordered_map<std::string, Config*>& configs = config->GetConfigs();
	for (auto iter = configs.begin(); iter != configs.end(); ++iter)
		if (GetMaterial(iter->first) == nullptr)
			LoadMaterial(iter->second, iter->first);
}

// Returns the projection matrix of the scene's camera
const glm::mat4& Scene::GetProjectionMatrix(float aspect)
{
	return GetCamera()->GetProjection(aspect);
}

// Returns the view matrix of the scene's camera
const glm::mat4& Scene::GetViewMatrix()
{
	return GetCamera()->GetView();
}

// Constructs the scene from the given file
Scene::Scene(State* state)
	: mState(state)
{
	// Only state needs to be assigned 
	mGrid = new PGrid(GRID_OBJ, 5, 1.0f, new Material(glm::vec3(0.2f)), 2, true, glm::vec3(0.0f));
	mViewAxisHandle = new PHandle(CAMERA_AXIS_HANDLE, 45.0f, 6, false, glm::vec3(50, 50, 0));
	mTransformHandle = new PHandle(TRANSFORM_HANDLE, 0.5f, 6, true, glm::vec3(0.0f));
}

Scene::~Scene()
{
	if (mMainCamera != nullptr)
		delete mMainCamera;

	for (auto iter = mShaderList.begin(); iter != mShaderList.end(); ++iter)
	{
		if (iter->second != nullptr)
			delete iter->second;
	}
	for (auto iter = mRenderList.begin(); iter != mRenderList.end(); ++iter)
	{
		if (iter->second != nullptr) {
			iter->second->Cleanup();
			delete iter->second;
		}
	}
	for (auto iter = mPreRenderList.begin(); iter != mPreRenderList.end(); ++iter)
	{
		if (iter->second != nullptr) {
			iter->second->Cleanup();
			delete iter->second;
		}
	}
	for (auto iter = mMaterialList.begin(); iter != mMaterialList.end(); ++iter)
		if (iter->second != nullptr)
			delete iter->second;

	for (auto iter = mTextureList.begin(); iter != mTextureList.end(); ++iter)
		if (iter->second != nullptr)
			delete iter->second;

	for (unsigned int i = 0; i < mMeshList.size(); i++)
		if (mMeshList[i] != nullptr)
			delete mMeshList[i];

	mShaderList.clear();
	mPreRenderList.clear();
	mRenderList.clear();
	mMaterialList.clear();
	mTextureList.clear();
	mMeshList.clear();
}