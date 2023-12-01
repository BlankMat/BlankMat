#pragma once
#include "utils.h"
#include "interfaces/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "interaction/selection.h"
#include "rendering/scene.h"
#include "rendering/camera.h"
#include "tools/state.h"
#include <map>

/// <summary>
/// 
/// </summary>
class GUIDebugToolsWindow : public IGUIWindow
{
protected:
	State* mState = nullptr;
	Scene* mScene = nullptr;
public:
	void Draw() override
	{
		// Don't draw disabled GUI
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Debug Tools", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(("Frame Time : " + mState->GetTimer()->GetFrameTime() + " [" + mState->GetTimer()->GetFPS() + " FPS]").c_str());
			ImGui::Text(("Delta Time: " + std::to_string(mState->GetTimer()->GetDeltaTime())).c_str());
			// Shading settings
			ImGui::Text("Shading");
			// Select shader
			const auto& shaderList = mScene->GetShaders()->Data();
			std::map<std::string, Shader*> sortedShaders;
			for (auto iter = shaderList.begin(); iter != shaderList.end(); ++iter)
				sortedShaders.emplace(iter->first, iter->second);
			std::string curShader = mScene->GetCurShader();
			if (ImGui::BeginListBox("Shader"))
			{
				for (auto iter = sortedShaders.begin(); iter != sortedShaders.end(); ++iter)
					GUIWindowUtils::Selectable(iter->first, curShader, iter->first);
				ImGui::EndListBox();

				// Only change the shader if it actually changed
				if (mScene->GetCurShader() != curShader)
				{
					mScene->UseShader(curShader);
				}
			}

			// Choose parts of materials
			ImGui::Separator();
			mState->enableDiffuseMap.Display();
			mState->enableAmbientMap.Display();
			mState->enableSpecularMap.Display();
			mState->enableNormalMap.Display();
			mState->enableHeightMap.Display();
			mState->enableAlphaMap.Display();
			mState->enableShadows.Display();
			mState->enableGrid.Display();
			mState->drawByMaterial.Display();
			mState->collapseMeshNodes.Display();

			// Input debug
			ImGui::Separator();
			ImGui::Text("Input");
			ImGui::InputInt("Current X", &mState->mouseCurX, 0);
			ImGui::InputInt("Current Y", &mState->mouseCurY, 0);
			ImGui::InputInt("Previous X", &mState->mousePrevX, 0);
			ImGui::InputInt("Previous Y", &mState->mousePrevY, 0);

			// Debug settings
			ImGui::Separator();
			ImGui::Text("Debug settings");
			mState->isDiscoLight.Display();
			mState->isRotatingLight.Display();
		}
		ImGui::End();
	}

	GUIDebugToolsWindow(State* state, Scene* scene, bool isEnabled)
		: mState(state), mScene(scene)
	{
		mType = GUI::DEBUG_TOOLS;
		mIsEnabled = isEnabled;
	}
};