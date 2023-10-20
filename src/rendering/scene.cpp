#include "scene.h"
#include "primitives/pLightCube.h"
#include "files/modelReader.h"

void Scene::Draw(Window* window, Shader* shader)
{
	// Set lighting uniforms
	Shader* curShader = GetShader(mCurShader);
	if (mCurShader == "")
		UseShader("default");

	GetLight()->UpdateShader(shader);
	shader->SetVec3("viewPos", GetCamera()->GetPos());

	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();

	// Draw all of the scene
	UseShader(mCurShader);
	mRootNode->Draw(mShader, mState, mDefaultMat, viewProj);

	// Draw lights
	if (mGlobalLight != nullptr)
		mGlobalLight->Draw(mShader, mState, mDefaultMat, viewProj);

	// Draw pre-renderables
	for (auto iter = mPreRenderList.begin(); iter != mPreRenderList.end(); ++iter)
		if (iter->second != nullptr)
			iter->second->Draw(mShader, mState, mDefaultMat, viewProj);

	// Draw post-renderables
	for (auto iter = mRenderList.begin(); iter != mRenderList.end(); ++iter)
	{
		// Render camera axis handle with inverse view matrix
		if (iter->first == CAMERA_AXIS_HANDLE) {
			iter->second->SetRot(GetRotationDegrees(mMainCamera->GetDir()));
			iter->second->Draw(mShader, mState, mDefaultMat, glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -100.0f, 100.0f));
		}
		else if (iter->second != nullptr) {
			iter->second->Draw(mShader, mState, mDefaultMat, viewProj);
		}
	}
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