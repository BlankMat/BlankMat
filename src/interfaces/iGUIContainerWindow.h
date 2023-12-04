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

    virtual void DisplayListItem(const std::string& name, T* item, T*& selection)
    {
        if (mMustSelect)
            GUIWindowUtils::Selectable(name, selection, item);
        else
            GUIWindowUtils::Deselectable(name, selection, item);
    }

    void ListContainer(IContainer<T>* container)
    {
        T* selection = container->GetSelectedItem();
        const auto& data = container->Data();
        for (auto iter = data.begin(); iter != data.end(); ++iter)
        {
            DisplayListItem(iter->first, iter->second, selection);
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