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

    virtual T* GetSelectedItem() = 0;

    virtual IContainer<T>* GetContainer() = 0;

    virtual void DisplaySelectedItem(T* selection) = 0;

    void ListContainer(IContainer<T>* container)
    {
        std::string selection = container->GetSelectedName();
        const auto& data = container->Data();
        for (auto iter = data.begin(); iter != data.end(); ++iter)
        {
            if (mMustSelect)
                GUIWindowUtils::Selectable(iter->first, selection, iter->first);
            else
                GUIWindowUtils::Deselectable(iter->first, selection, iter->first);
        }
        if (selection != container->GetSelectedName())
        {
            container->Select(selection);
        }
    }
public:
    void Draw() override
    {
        if (ImGui::Begin(mWindowName.c_str(), &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            DisplaySelectedItem(GetSelectedItem());
            ImGui::Separator();
            ListContainer(GetContainer());
        }
        ImGui::End();
    }
};