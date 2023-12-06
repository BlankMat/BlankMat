#pragma once
#include "interfaces/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "tools/state.h"
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

    virtual bool DisplayListItem(const std::string& name, T* item, T*& selection)
    {
        bool wasPressed = false;
        if (mMustSelect)
            GUIWindowUtils::Selectable(name, selection, item, &wasPressed);
        else
            GUIWindowUtils::Deselectable(name, selection, item, &wasPressed);
        return wasPressed;
    }

    void ListContainer(IContainer<T>* container)
    {
        bool wasPressed = false;
        T* selection = container->GetSelectedItem();
        const auto& data = container->Data();
        for (auto iter = data.begin(); iter != data.end(); ++iter)
        {
            if (DisplayListItem(iter->first, iter->second, selection))
                wasPressed = true;
        }
        if (wasPressed)
        {
            SelectItem(selection);
        }
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

            /*
            bool enterPressed = false;
            curValue = GUIWindowUtils::InputText("Name##NewItemName", curValue, &enterPressed);

            // Don't allow creating of items with duplicate names
            if (container->Contains(curValue))
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Text((mItemName + " with name " + curValue + " already exists.").c_str());
                ImGui::PopStyleColor();
            }
            // If the text is valid and enter was pressed
            else if (curValue != "" && enterPressed)
            {
                AddNewItem(curValue);
                curValue = "";
                mIsAddingItem = false;
            }
            */
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