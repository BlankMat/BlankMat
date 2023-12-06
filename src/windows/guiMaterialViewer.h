#pragma once
#include "interfaces/iGUIContainerWindow.h"

class GUIMaterialViewer : public IGUIContainerWindow<Material>
{
private:
	void AddNewItem(const std::string& name) override
	{
		mScene->AddMaterial(name, new Material(name, "", glm::vec3(1.0f), mScene->GetTextures(), false));
	}

	IContainer<Material>* GetContainer() override
	{
		return mScene->GetMaterials();
	}

	void DisplaySelectedItem() override
	{
		Material* selection = mScene->GetMaterials()->GetSelectedItem();
		if (selection != nullptr)
			return;

		ImGui::Text("No material selected");
	}

	void SelectItem(Material* selection) override
	{
		mState->GetSel()->SelectElement(selection);
	}

	bool DisplayListItem(const std::string& name, Material* item, Material*& selection) override
	{
		if (item->IsInternal())
			return false;

		return GUIWindowUtils::MaterialSelect(name, item, selection, 5.0f);
	}
public:
	GUIMaterialViewer(State* state, Scene* scene, bool isEnabled)
	{
		mState = state;
		mScene = scene;
		mType = GUI::MATERIAL_VIEWER;
		mIsEnabled = isEnabled;
		mMustSelect = false;
		mWindowName = "Material Viewer";
	}
};