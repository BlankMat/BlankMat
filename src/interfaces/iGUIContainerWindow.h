#pragma once
#include "interfaces/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "interaction/state.h"
#include "rendering/scene.h"

template<typename T>
class IGUIContainerWindow : public IGUIWindow
{
protected:
    State* mState = nullptr;
    Scene* mScene = nullptr;
    bool mMustSelect = false;
    std::string mWindowName = "";
    std::string mItemName = "";
    
    bool mIsAddingItem = false;
    std::string mAddItemName = "";

    virtual IContainer<T>* GetContainer() = 0;

    virtual void DisplaySelectedItem() = 0;

    virtual void AddNewItem(const std::string& name) = 0;

    virtual void SelectItem(T* selection) = 0;

    virtual void DisplayDeleteItem(T* item, T*& deleteItem = nullptr)
    {
        // If the item can be removed, show delete icon
        if (GetContainer()->IsDeleteable(item))
        {
            ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetTextLineHeight());
            if (ImGui::Button("-##RemoveItem"))
            {
                if (deleteItem != nullptr)
                    deleteItem = item;
            }
        }
    }

    virtual bool DisplayListItem(const std::string& name, T* item, T*& selection, T*& deleteItem = nullptr)
    {
        bool wasPressed = false;
        if (mMustSelect)
            GUIWindowUtils::Selectable(name, selection, item, &wasPressed);
        else
            GUIWindowUtils::Deselectable(name, selection, item, &wasPressed);

        DisplayDeleteItem(item, deleteItem);
        return wasPressed;
    }

    void ListContainer(IContainer<T>* container)
    {
        bool wasPressed = false;
        T* deleteItem = nullptr;
        T* selection = container->GetSelectedItem();

        // List all elements in the container
        const auto& data = container->Data();
        for (auto iter = data.cbegin(); iter != data.cend(); ++iter)
        {
            if (DisplayListItem(iter->first, iter->second, selection, deleteItem))
                wasPressed = true;
        }

        // If an item was selected for deletion, delete it after the loop is complete
        if (deleteItem != nullptr)
        {
            container->Remove(deleteItem);
            mState->GetSel()->DeselectElement();
        }

        // Select the item that was pressed
        if (wasPressed)
        {
            SelectItem(selection);
        }

        // If a new item was selected, set that item to be the container's selected item
        if (selection != container->GetSelectedItem())
        {
            container->Select(selection);
        }

        // Support adding items to list
        ImGui::Separator();
        ImGui::Selectable(("+ Add " + mItemName + "##AddContainerItem").c_str(), &mIsAddingItem);
        if (mIsAddingItem)
        {
            if (GUIWindowUtils::InputName(mItemName, "", mAddItemName, container))
            {
                AddNewItem(mAddItemName);
                mAddItemName = "";
                mIsAddingItem = false;
            }
        }
    }
public:
    void Draw() override
    {
        if (ImGui::Begin(mWindowName.c_str(), &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            DisplaySelectedItem();
            ImGui::Separator();
            ListContainer(GetContainer());
        }
        ImGui::End();
    }
};