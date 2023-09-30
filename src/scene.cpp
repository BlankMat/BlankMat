#include "scene.h"

void Scene::Draw(Window* window)
{
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();
	if (mCurShader == "")
		UseShader(DEFAULT_SHADER);
	for (auto iter = mPreRenderList.begin(); iter != mPreRenderList.end(); ++iter)
		if (iter->second != nullptr)
			iter->second->Draw(viewProj);
	if (mCurModel != nullptr)
		mCurModel->Draw(viewProj);
	if (mGlobalLight != nullptr)
		mGlobalLight->Draw(viewProj);
	for (auto iter = mRenderList.begin(); iter != mRenderList.end(); ++iter)
	{
		// Render camera axis handle with inverse view matrix
		if (iter->first == CAMERA_AXIS_HANDLE) {
			iter->second->SetRot(mMainCamera->GetRotationDegrees());
			iter->second->Draw(glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -100.0f, 100.0f));
		}
		else if (iter->second != nullptr) {
			iter->second->Draw(viewProj);
		}
	}
}

void Scene::UseShader(std::string name)
{
	if (mShaderList.find(name) != mShaderList.end() && mShaderList[name] != nullptr)
	{
		mShaderList[name]->Use();
		mCurShader = name;
	}
}

void Scene::CreateShader(std::string name, bool loadGeom)
{
	if (mShaderList.find(name) == mShaderList.end())
		mShaderList.emplace(name, new Shader(name, loadGeom));
}

void Scene::CreateShader(std::string name, std::string source, bool loadGeom)
{
	if (mShaderList.find(name) == mShaderList.end())
		mShaderList.emplace(name, new Shader(source, loadGeom));
}

Scene::Scene()
{
	mMainCamera = nullptr;
	mGlobalLight = nullptr;
	mCurModel = nullptr;
	mShaderList = std::unordered_map<std::string, Shader*>();
}

Scene::~Scene()
{
	if (mMainCamera != nullptr)
		delete mMainCamera;
	if (mGlobalLight != nullptr)
		delete mGlobalLight;
	if (mCurModel != nullptr)
		delete mCurModel;

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
	mShaderList.clear();
	mPreRenderList.clear();
	mRenderList.clear();
}