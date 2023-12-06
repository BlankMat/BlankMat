#pragma once
#include "interfaces/iGUIContainerWindow.h"

class GUITextureViewer : public IGUIContainerWindow<Texture>
{
protected:
    void AddNewItem(const std::string& name) override
    {
        mScene->AddTexture(name, new Texture(name, "", TextureType::DIFFUSE, glm::vec3(1.0f), false));
    }

    IContainer<Texture>* GetContainer() override
    {
        return mScene->GetTextures();
    }

    void DisplaySelectedItem() override
    {
        Texture* selection = mScene->GetTextures()->GetSelectedItem();
        if (selection != nullptr)
            return;

        ImGui::Text("No texture selected");
    }
    
    void SelectItem(Texture* selection) override
    {
        mState->GetSel()->SelectElement(selection);
    }

    bool DisplayListItem(const std::string& name, Texture* item, Texture*& selection) override
    {
        return GUIWindowUtils::TextureSelect(name, item, selection);
    }

public:
    GUITextureViewer(State* state, Scene* scene, bool isEnabled)
    {
        mState = state;
        mScene = scene;
        mType = GUI::TEXTURE_VIEWER;
        mIsEnabled = isEnabled;
        mMustSelect = false;
        mWindowName = "Texture Viewer";
    }
};