#include "scene.h"
#include "primitives/pLightCube.h"
#include "interaction/selection.h"
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

// Sets the view axis handle to the given object
void Scene::SetViewAxisHandle(IEntity* viewHandle)
{
	mViewAxisHandle = viewHandle;
}

// Sets the grid object
void Scene::SetGrid(IEntity* grid)
{
	mGrid = grid;
}

// Sets the transform handle object
void Scene::SetTransformHandle(IEntity* handle)
{
	mTransformHandle = handle;
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

// Constructs the scene from the given file
Scene::Scene(State* state)
{
	mCurShader = "";
	mState = state;
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