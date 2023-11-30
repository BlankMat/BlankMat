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
			mGrid->Draw(flatShader, mState, GetDefaultMaterial(), viewProj, true);

		mLights->Draw(flatShader, mState, GetDefaultMaterial(), viewProj, true);
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
				mat->LoadShaderTextures(mState, GetDefaultMaterial());
				unsigned int shadowIndex = mat->UpdateShader(shader);
				glActiveTexture(GL_TEXTURE0 + shadowIndex);
				glBindTexture(GL_TEXTURE_2D, mState->depthMap);
				shader->SetInt("shadowMap", mState->depthMapFBO);
				glActiveTexture(GL_TEXTURE0);
			}

			// Draw mesh
			iter->second->Draw(shader, mState, GetDefaultMaterial(), viewProj, mat == nullptr);
		}
	}
	// Default draw
	else
	{
		mRootNode->Draw(shader, mState, GetDefaultMaterial(), viewProj, true);
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
				iter->second->Draw(curShader, mState, GetDefaultMaterial(), viewProj, true);
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
			mViewAxisHandle->Draw(flatShader, mState, GetDefaultMaterial(), GetViewAxisProjection(window), true);
		}

		if (mTransformHandle != nullptr)
		{
			mState->GetSel()->UpdateTransformHandle();
			mTransformHandle->Draw(flatShader, mState, GetDefaultMaterial(), viewProj, true);
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
	Texture* kd = LoadTexture(TextureType::DIFFUSE, config->GetString("map_kd"), "default_diffuse");
	Texture* ka = LoadTexture(TextureType::AMBIENT, config->GetString("map_ka"), "default_ambient");
	Texture* ks = LoadTexture(TextureType::SPECULAR, config->GetString("map_ks"), "default_specular");
	Texture* normal = LoadTexture(TextureType::NORMAL, config->GetString("map_bump"), "default_normal");
	Texture* ns = LoadTexture(TextureType::HEIGHT, config->GetString("map_ns"), "default_height");
	Texture* d = LoadTexture(TextureType::ALPHA, config->GetString("map_d"), "default_alpha");

	// Create material
	material = new Material(name, config, kd, ka, ks, normal, ns, d);
	AddMaterial(name, material);
	return material;
}

// Loads the given texture or returns the existing one
Texture* Scene::LoadTexture(TextureType type, const std::string& path, const std::string& defaultName)
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
	Shader* shader = mShaders->GetItem(name);
	if (shader != nullptr)
	{
		mCurShader = name;
		mShader = shader;
		mShader->Use();
	}
}

// Creates a shader for the scene with the given name from the source file of the given name
Shader* Scene::CreateShader(const std::string& name, bool loadGeom)
{
	if (mShaders->Contains(name))
		return mShaders->GetItem(name);
	return mShaders->Add(name, new Shader(name, loadGeom, name));
}

// Creates a shader for the scene with the given name, loading it from a different source than the name
Shader* Scene::CreateShader(const std::string& name, const std::string& source, bool loadGeom)
{
	if (mShaders->Contains(name))
		return mShaders->GetItem(name);
	return mShaders->Add(name, new Shader(source, loadGeom, name));
}

// Creates a shader for the scene with the given name, loading it from a config
Shader* Scene::CreateShader(const std::string& name, Config* config)
{
	if (mShaders->Contains(name))
		return mShaders->GetItem(name);
	return mShaders->Add(name, new Shader(config->GetString("file"), config->GetBool("hasGeomShader"), name));
}

// Updates the scene's material render list
void Scene::UpdateRenderList()
{
	// Clear the render list
	for (auto iter = mMeshRenderList.begin(); iter != mMeshRenderList.end(); ++iter)
		iter->second->ClearData();

	// Add all meshes to their appropriate render list
	for (auto iter = mMeshes->Data().begin(); iter != mMeshes->Data().end(); ++iter)
		SetEntityMaterial(iter->second, iter->second->GetMaterial());
}

// Returns the texture container of the scene
TextureContainer* Scene::GetTextures()
{
	return mTextures;
}

// Returns the material container of the scene
MaterialContainer* Scene::GetMaterials()
{
	return mMaterials;
}

// Returns the light container of the scene
LightContainer* Scene::GetLights()
{
	return mLights;
}

// Returns the camera container of the scene
CameraContainer* Scene::GetCameras()
{
	return mCameras;
}

// Returns the shader container of the scene
ShaderContainer* Scene::GetShaders()
{
	return mShaders;
}

// Returns the mesh container of the scene
MeshContainer* Scene::GetMeshes()
{
	return mMeshes;
}

// Returns the entity container of the scene
EntityContainer* Scene::GetEntities()
{
	return mEntities;
}

// Returns the current shader
const std::string Scene::GetCurShader()
{
	return mCurShader;
}

// Returns the scene's camera
Camera* Scene::GetCamera()
{
	return mCameras->GetSelectedItem();
}

// Returns the scene's light
Light* Scene::GetLight()
{
	return mLights->GetSelectedItem();
}

// Returns the scene's name
std::string Scene::GetName()
{
	return mName;
}

// Returns the scene's directory
std::string Scene::GetDirectory()
{
	return mDirectory;
}

// Returns the shader with the given name
Shader* Scene::GetShader(const std::string& name)
{
	return mShaders->GetItem(name);
}

// Returns the material with the given name
Material* Scene::GetMaterial(const std::string& name)
{
	return mMaterials->GetItem(name);
}

// Returns the default material
Material* Scene::GetDefaultMaterial()
{
	return mMaterials->GetDefault();
}

// Returns the material with the given name
Texture* Scene::GetTexture(const std::string& name)
{
	return mTextures->GetItem(name);
}

// Returns the entity with the given name
IEntity* Scene::GetEntity(const std::string& name)
{
	/*
	if (mPreRenderList.find(name) != mPreRenderList.end())
		return mPreRenderList[name];
	else if (mRenderList.find(name) != mRenderList.end())
		return mRenderList[name];
	*/
	return nullptr;
}

// Returns the transform handle
IEntity* Scene::GetTransformHandle()
{
	return mTransformHandle;
}

// Returns the state of the application
State* Scene::GetState()
{
	return mState;
}

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
	mMeshes->Add(mesh->GetName(), mesh);
	SetEntityMaterial(mesh, mesh->GetMaterial());
}

// Adds the given camera to the scene
void Scene::AddCamera(const std::string& name, Camera* camera, bool select)
{
	mCameras->Add(name, camera);
	if (select)
		mCameras->Select(name);
}

// Adds the given camera to the scene
void Scene::AddLight(const std::string& name, Light* light, bool select)
{
	mLights->Add(name, light);
	if (select)
		mLights->Select(name);
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

// Sets the scene's camera to the given camera
void Scene::SetCamera(const std::string& camera)
{
	mCameras->Select(camera);
}

// Sets the scene's light to the given light
void Scene::SetLight(const std::string& light)
{
	mLights->Select(light);
}

// Sets the scene's name
void Scene::SetName(const std::string& name) { mName = name; }

// Sets the scene's directory
void Scene::SetDirectory(const std::string& dir) { mDirectory = dir; }

// Adds an entity to the scene's render list
IEntity* Scene::AddEntity(const std::string& shaderName, IEntity* entity, bool preRender)
{
	if (mEntityList.find(shaderName) == mEntityList.begin())
		mEntityList.emplace(shaderName, new EntityContainer());
	mEntityList[shaderName]->Add(entity->GetName(), entity);
	return entity;
}

// Adds a texture to the scene's texture list
Texture* Scene::AddTexture(const std::string& name, Texture* texture)
{
	return mTextures->Add(name, texture);
}

// Adds a material to the scene's material list
Material* Scene::AddMaterial(const std::string& name, Material* material)
{
	return mMaterials->Add(name, material);
}

// Sets the material of the given entity
void Scene::SetEntityMaterial(IEntity* entity, Material* material)
{
	std::string prevMatName = entity->GetMaterial()->name;
	std::string matName = material->name;

	// If the material is already stored in the render list, remove it
	if (mMeshRenderList.find(prevMatName) != mMeshRenderList.end())
	{
		if (mMeshRenderList[prevMatName]->GetItem(entity->GetName()) != nullptr)
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

// Clears the scene completely
void Scene::Clear()
{
	mTextures->Clear();
	mMaterials->Clear();
	mLights->Clear();
	mCameras->Clear();
	mMeshes->Clear();
	mEntities->Clear();
	mMeshRenderList.clear();
	mRootNode->Clear();
}

// Constructs the scene from the given file
Scene::Scene(State* state)
	: mState(state)
{
	mTextures = new TextureContainer();
	mMaterials = new MaterialContainer(mTextures);

	mCameras = new CameraContainer(state->GetActionStack());
	mLights = new LightContainer();
	mShaders = new ShaderContainer();
	mMeshes = new MeshContainer();
	mEntities = new EntityContainer();

	mMaterials->AddMaterial(new Material("gray", glm::vec3(0.2f), mTextures));
	mGrid = new PGrid(GRID_OBJ, 5, 1.0f, mMaterials->GetItem("gray"), 2, true, glm::vec3(0.0f));
	mViewAxisHandle = new PHandle(CAMERA_AXIS_HANDLE, 45.0f, 6, false, mMaterials, mTextures, glm::vec3(50, 50, 0));
	mTransformHandle = new PHandle(TRANSFORM_HANDLE, 0.5f, 6, true, mMaterials, mTextures, glm::vec3(0.0f));
}

Scene::~Scene()
{
	Clear();
	mShaders->Clear();

	delete mGrid;
	delete mViewAxisHandle;
	delete mTransformHandle;
	delete mTextures;
	delete mMaterials;
	delete mLights;
	delete mCameras;
	delete mShaders;
	delete mMeshes;
	delete mEntities;
}