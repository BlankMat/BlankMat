#include "scene.h"
#include "primitives/pLightCube.h"
#include "files/modelReader.h"

// Renders the current scene
void Scene::Draw(Window* window, Shader* shader)
{
	// Calculate camera view projection
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();

	// Draw all of the scene
	GetLight()->UpdateShader(shader);
	shader->SetVec3("viewPos", GetCamera()->GetPos());
	mRootNode->Draw(shader, mState, mDefaultMat, viewProj);

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
				iter->second->Draw(curShader, mState, mDefaultMat, viewProj);
			}
		}
	}

	// Draw flat objects
	Shader* flatShader = GetShader("flat");
	if (flatShader != nullptr)
	{
		flatShader->Use();
		if (mViewAxisHandle != nullptr)
		{
			mViewAxisHandle->SetRot(GetRotationDegrees(GetCamera()->GetDir()));
			mViewAxisHandle->Draw(flatShader, mState, mDefaultMat, glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -100.0f, 100.0f));
		}
			
		if (mGlobalLight != nullptr)
			mGlobalLight->Draw(shader, mState, mDefaultMat, viewProj);
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
	mRootNode->Draw(shader, mState, mDefaultMat, viewProj);
}

// Adds the given node to the scene
void Scene::AddNode(Node* node)
{
	mRootNode->AddChild(node);
}

// Adds the given mesh to the scene
void Scene::AddMesh(Mesh* mesh)
{
	mRootNode->AddMesh(mesh);
}

// Returns the root node
Node* Scene::GetRootNode()
{
	return mRootNode;
}

// Saves the global state in the scene
void Scene::SetState(State* state)
{
	mState = state;
}

// Loads the model at the given path
void Scene::LoadModel(const std::string& path, glm::vec3 startPos, glm::vec3 startRot, glm::vec3 startScale)
{
	ModelReader::LoadModel(this, path, startPos, startRot, startScale);
}

// Sets the view axis handle to the given object
void Scene::SetViewAxisHandle(IEntity* viewHandle)
{
	mViewAxisHandle = viewHandle;
}

// Constructs the scene from the given file
Scene::Scene()
{
	mCurShader = "";
	mMainCamera = nullptr;
	mGlobalLight = nullptr;
	mRootNode = nullptr;
	mRenderList = std::unordered_map<std::string, IEntity*>();
	mPreRenderList = std::unordered_map<std::string, IEntity*>();
	mShaderList = std::unordered_map<std::string, Shader*>();
	mMaterialList = std::unordered_map<std::string, Material*>();
	mTextureList = std::unordered_map<std::string, Texture*>();
	mMeshList = std::vector<Mesh*>();
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