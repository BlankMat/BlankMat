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

    virtual IContainer<T>* GetContainer() = 0;

    virtual void DisplaySelectedItem() = 0;

    virtual bool DisplayListItem(const std::string& name, T* item, T*& selection)
    {
        bool wasPressed = false;
        if (mMustSelect)
            GUIWindowUtils::Selectable(name, selection, item, &wasPressed);
        else
            GUIWindowUtils::Deselectable(name, selection, item, &wasPressed);
        return wasPressed;
    }

    virtual void SelectItem(T* selection) = 0;

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