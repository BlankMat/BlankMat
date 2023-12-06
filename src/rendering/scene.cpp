#include "rendering/scene.h"
#include "windows/window.h"

// Renders the current scene
void Scene::Draw(Window* window, Shader* shader)
{
	// Calculate camera view projection
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();

	// Draw flat objects
	Shader* unlitShader = GetShader("unlit");
	if (unlitShader != nullptr)
	{
		unlitShader->Use();
		if (mGrid != nullptr && mState->enableGrid)
			mGrid->Draw(unlitShader, mState, GetDefaultMaterial(), viewProj, true);

		mLights->Draw(unlitShader, mState, GetDefaultMaterial(), viewProj, true);
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
			if (mMode == LightingMode::WIREFRAME)
				glLineWidth(WIREFRAME_LINE);
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
	if (unlitShader != nullptr)
	{
		unlitShader->Use();
		if (mViewAxisHandle != nullptr)
		{
			mViewAxisHandle->SetRot(GetRotationDegrees(GetCamera()->GetDir()));
			mViewAxisHandle->Draw(unlitShader, mState, GetDefaultMaterial(), GetViewAxisProjection(window), true);
		}

		if (mTransformHandle != nullptr)
		{
			mState->GetSel()->UpdateTransformHandle();
			mTransformHandle->Draw(unlitShader, mState, GetDefaultMaterial(), viewProj, true);
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

// Enables or disables wireframe
void Scene::HandleWireframe()
{
	if (mMode == LightingMode::WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Disable culling
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Enable culling
		glEnable(GL_CULL_FACE);

		// Enable depth buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

// Loads the material of the given config. Must be bottom-level config
Material* Scene::LoadMaterial(Config* config, const std::string& name, const std::string& scope)
{
	// Don't handle null configs
	if (config == nullptr)
		return nullptr;

	// If the material exists already, return it
	Material* material = GetMaterial(name);
	if (material != nullptr)
		return material;

	// Create textures
	Texture* kd = LoadTexture(TextureType::DIFFUSE, config->GetString("map_kd"), scope, "default_diffuse");
	Texture* ka = LoadTexture(TextureType::AMBIENT, config->GetString("map_ka"), scope, "default_ambient");
	Texture* ks = LoadTexture(TextureType::SPECULAR, config->GetString("map_ks"), scope, "default_specular");
	Texture* normal = LoadTexture(TextureType::NORMAL, config->GetString("map_bump"), scope, "default_normal");
	Texture* ns = LoadTexture(TextureType::HEIGHT, config->GetString("map_ns"), scope, "default_height");
	Texture* d = LoadTexture(TextureType::ALPHA, config->GetString("map_d"), scope, "default_alpha");

	// Create material
	return AddMaterial(name, new Material(name, scope, config, kd, ka, ks, normal, ns, d));
}

// Loads the given texture or returns the existing one
Texture* Scene::LoadTexture(TextureType type, const std::string& path, const std::string& scope, const std::string& defaultName)
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
	if (texture != nullptr && texture->mType == type)
		return texture;

	// If the texture is not loaded, load the texture and store it
	return AddTexture(name, new Texture(name, scope, type, FileSystem::GetPath(TEXTURE_DIR), pathToLoad));
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
	return mShaders->Add(name, new Shader(name, loadGeom, name), false);
}

// Creates a shader for the scene with the given name, loading it from a different source than the name
Shader* Scene::CreateShader(const std::string& name, const std::string& source, bool loadGeom)
{
	if (mShaders->Contains(name))
		return mShaders->GetItem(name);
	return mShaders->Add(name, new Shader(source, loadGeom, name), false);
}

// Creates a shader for the scene with the given name, loading it from a config
Shader* Scene::CreateShader(const std::string& name, Config* config)
{
	if (mShaders->Contains(name))
		return mShaders->GetItem(name);
	return mShaders->Add(name, new Shader(config->GetString("file"), config->GetBool("hasGeomShader"), name), false);
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

// Returns the texture with the given name
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

// Returns the lighting mode of the application
LightingMode Scene::GetLightingMode()
{
	return mMode;
}

// Adds the given node to the scene
void Scene::AddNode(Node* node)
{
	mRootNode->AddChild(node);
}

// Adds the given mesh to the scene
Mesh* Scene::AddMesh(Mesh* mesh, Node* parent)
{
	if (parent == nullptr)
		parent = mRootNode;
	parent->AddMesh(mesh);
	SetEntityMaterial(mesh, mesh->GetMaterial());
	return mMeshes->Add(mesh->GetScopedName(), mesh, false, true);
}

// Adds the given camera to the scene
Camera* Scene::AddCamera(const std::string& name, Camera* camera, bool select)
{
	return mCameras->Add(name, camera, false, select);
}

// Adds the given camera to the scene
Light* Scene::AddLight(const std::string& name, Light* light, bool select)
{
	return mLights->Add(name, light, false, select);
}

// Adds an entity to the scene's render list
IEntity* Scene::AddEntity(const std::string& shaderName, IEntity* entity, bool preRender)
{
	if (!mEntityList.contains(shaderName))
		mEntityList.emplace(shaderName, new EntityContainer());
	mEntityList[shaderName]->Add(entity->GetScopedName(), entity, false);
	return entity;
}

// Adds a texture to the scene's texture list
Texture* Scene::AddTexture(const std::string& name, Texture* texture)
{
	return mTextures->Add(name, texture, false, true);
}

// Adds a material to the scene's material list
Material* Scene::AddMaterial(const std::string& name, Material* material)
{
	return mMaterials->Add(name, material, false, true);
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
void Scene::SetName(const std::string& name)
{
	mName = name;
}

// Sets the scene's directory
void Scene::SetDirectory(const std::string& dir)
{
	mDirectory = dir;
}

// Sets the lighting mode of the scene
void Scene::SetLightingMode(LightingMode mode)
{
	mMode = mode;
	switch (mode)
	{
	case LightingMode::UNLIT:
		UseShader("unlit");
		break;
	case LightingMode::FLAT:
		UseShader("flat");
		break;
	case LightingMode::LIT:
		UseShader("lit");
		break;
	case LightingMode::TEXTURED:
		UseShader("textured");
		break;
	case LightingMode::WIREFRAME:
		UseShader("wireframe");
		break;
	default:
		break;
	}
	HandleWireframe();
}

// Sets the material of the given entity
void Scene::SetEntityMaterial(IEntity* entity, Material* material)
{
	// Don't set material of empty entity or set material to empty
	if (entity == nullptr || material == nullptr)
		return;

	std::string prevMatName = (entity->GetMaterial() != nullptr) ? entity->GetMaterial()->GetScopedName() : "";
	std::string matName = material->GetScopedName();

	// If the material is already stored in the render list, remove it
	if (prevMatName != "" && mMeshRenderList.contains(prevMatName))
	{
		if (mMeshRenderList[prevMatName]->GetItem(entity->GetScopedName()) != nullptr)
			mMeshRenderList[prevMatName]->Remove(entity->GetScopedName());
	}
	entity->SetMaterial(material);

	// Add the material to the appropriate render list
	if (!mMeshRenderList.contains(material->GetScopedName()))
	{
		mMeshRenderList.emplace(material->GetScopedName(), new EntityContainer());
	}
	mMeshRenderList[material->GetScopedName()]->Add(entity->GetScopedName(), entity, false);
}

// Loads the materials from the given config
void Scene::LoadMaterials(Config* config)
{
	// Load the default material if it exists
	LoadMaterial(config->GetConfig("default"), "default", "");

	// Load the rest of the materials
	const std::unordered_map<std::string, Config*>& configs = config->GetConfigs();
	for (auto iter = configs.begin(); iter != configs.end(); ++iter)
		if (GetMaterial(iter->first) == nullptr)
			LoadMaterial(iter->second, iter->first, "");
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
	if (mRootNode != nullptr)
		mRootNode->Clear();

	mMeshRenderList.clear();
}

// Clears all items that would render
void Scene::ClearRendering()
{
	mTextures->Clear();
	mMaterials->Clear();
	mMeshes->Clear();

	if (mRootNode != nullptr)
		mRootNode->Clear();
	mMeshRenderList.clear();
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

	mMaterials->AddMaterial(new Material("gray", "", glm::vec3(0.2f), mTextures));
	mGrid = new PGrid(GRID_OBJ, "", 5, 1.0f, mMaterials->GetItem("gray"), 2, true, glm::vec3(0.0f));
	mViewAxisHandle = new PHandle(CAMERA_AXIS_HANDLE, "", 45.0f, 6, false, mMaterials, mTextures, glm::vec3(50, 50, 0));
	mTransformHandle = new PHandle(TRANSFORM_HANDLE, "", 0.5f, 6, true, mMaterials, mTextures, glm::vec3(0.0f));
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