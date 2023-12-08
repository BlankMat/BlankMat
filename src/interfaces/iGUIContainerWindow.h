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

    virtual bool ShouldDisplayItem(T* item)
    {
        return true;
    }

    virtual bool DisplayDeleteItem(const std::string& name, T* item)
    {
        ImVec2 size = ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight());
        // If the item can be removed, show delete icon
        if (GetContainer()->IsDeleteable(item))
        {
            return ImGui::Selectable((" - ##RemoveItem" + name).c_str(), false, ImGuiSelectableFlags_None, size);
        }
        else 
        {
            ImGui::Selectable((".##DontRemoveItem" + name).c_str(), false, ImGuiSelectableFlags_Disabled, size);
            return false;
        }
    }

    virtual bool DisplayListItem(const std::string& name, T* item, T*& selection)
    {
        bool wasPressed = false;
        if (mMustSelect)
            GUIWindowUtils::Selectable(name, selection, item, &wasPressed, glm::vec2(0.0f, ImGui::GetTextLineHeight()));
        else
            GUIWindowUtils::Deselectable(name, selection, item, &wasPressed, glm::vec2(0.0f, ImGui::GetTextLineHeight()));
        return wasPressed;
    }

    void ListContainer(IContainer<T>* container)
    {
        bool wasPressed = false;
        T* deleteItem = nullptr;
        T* selection = container->GetSelectedItem();

        // Define column layout
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() - ImGui::GetTextLineHeightWithSpacing() * 2.5f);
        ImGui::SetColumnWidth(1, ImGui::GetTextLineHeightWithSpacing() * 2.0f);

        // List all elements in the container
        const auto& data = container->Data();
        for (auto iter = data.cbegin(); iter != data.cend(); ++iter)
        {
            if (DisplayListItem(iter->first, iter->second, selection))
                wasPressed = true;
        }

        // List delete buttons for all applicable items
        ImGui::NextColumn();
        for (auto iter = data.cbegin(); iter != data.cend(); ++iter)
        {
            if (ShouldDisplayItem(iter->second) && DisplayDeleteItem(iter->first, iter->second))
                deleteItem = iter->second;
        }

        // If an item was selected for deletion, delete it after the loop is complete
        if (deleteItem != nullptr)
        {
            mScene->DeleteSelectable(deleteItem);
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
        ImGui::Columns(1, nullptr, false);
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