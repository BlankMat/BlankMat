#include "modelScene.h"

void ModelScene::Draw(Window* window)
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

ModelScene::ModelScene()
{
	mCurShader = "";
	mMainCamera = nullptr;
	mGlobalLight = nullptr;
	mCurModel = nullptr;
	mRenderList = std::unordered_map<std::string, IEntity*>();
	mPreRenderList = std::unordered_map<std::string, IEntity*>();
	mShaderList = std::unordered_map<std::string, Shader*>();
	mMaterialList = std::unordered_map<std::string, Material*>();
}

ModelScene::~ModelScene()
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
	for (auto iter = mMaterialList.begin(); iter != mMaterialList.end(); ++iter)
	{
		if (iter->second != nullptr)
			delete iter->second;
	}
	mShaderList.clear();
	mPreRenderList.clear();
	mRenderList.clear();
	mMaterialList.clear();
}