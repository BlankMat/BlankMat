#include "scene.h"

void Scene::Draw(Window* window)
{
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();
	if (mCurShader == "")
		UseShader("default");
	if (mCurModel != nullptr)
		mCurModel->Draw(viewProj);
	if (mGlobalLight != nullptr)
		mGlobalLight->Draw(viewProj);
	for (unsigned int i = 0; i < mRenderList.size(); i++)
		mRenderList[i]->Draw(viewProj);
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
	for (unsigned int i = 0; i < mRenderList.size(); i++)
	{
		if (mRenderList[i] != nullptr)
		{
			mRenderList[i]->Cleanup();
			delete mRenderList[i];
		}
	}
	mShaderList.clear();
	mRenderList.clear();
}