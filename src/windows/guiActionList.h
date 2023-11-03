#pragma once
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"
#include "tools/state.h"
#include "interaction/selection.h"
#include "interaction/actionStack.h"

class GUIActionList : public IGUIWindow
{
private:
	State* mState;
	Scene* mScene;
public:
	void Draw() override
	{
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Action List", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Get actions
			ActionStack* actions = mState->GetActionStack();
			ActionNode* curAction = actions->GetCurAction();
			ActionNode* tempAction = actions->GetAction(-1);
			if (curAction != nullptr)
			{
				// List all actions
				while (tempAction != nullptr)
				{
					GUIWindowUtils::Selectable(tempAction->GetName(), curAction, tempAction);
					tempAction = tempAction->mPrevNode;
				}
			}
			else
			{
				ImGui::Text("No actions recorded.");
			}
		}
		ImGui::End();
	}

	GUIActionList(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::ACTION_LIST;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};