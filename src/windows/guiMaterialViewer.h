#pragma once
#include "interfaces/iGUIContainerWindow.h"

class GUIMaterialViewer : public IGUIContainerWindow<Material>
{
private:
	void AddNewItem(const std::string& name) override
	{
		mState->GetSel()->SelectElement(mScene->AddMaterial(name, new Material(name, "", mScene->GetTextures())));
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

	bool DisplayListItem(const std::string& name, Material* item, Material*& selection, Material*& deleteItem) override
	{
		if (item->IsInternal() && name != "default")
			return false;

		bool wasSelected = GUIWindowUtils::MaterialSelect(name, item, selection, 5.0f);
		DisplayDeleteItem(item, deleteItem);
		return wasSelected;
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
		mItemName = "Material";
	}
};