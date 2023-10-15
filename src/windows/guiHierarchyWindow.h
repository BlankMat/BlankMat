#pragma once
#include "iGUIWindow.h"
#include "tools/state.h"
#include "rendering/scene.h"

class GUIHierarchyWindow : public IGUIWindow
{
protected:
    State* mState;
    Scene* mScene;

    // Recursively renders the node and its children
    void RenderNode(IEntity*& selEntity, Node* node, int depth)
    {
        // Mark depth of nodes
        std::string depthMarker = "";
        for (int k = 0; k < depth - 1; k++)
            depthMarker += "  ";

        // Mark this node
        bool isSelected = (node == selEntity);
        if (ImGui::Selectable((">" + depthMarker + node->GetName()).c_str(), &isSelected))
            selEntity = node;
        if (isSelected)
            ImGui::SetItemDefaultFocus();

        depthMarker += "  ";

        // Render all meshes of node
        for (unsigned int j = 0; j < node->GetMeshCount(); j++)
        {
            // Get mesh
            IEntity* mesh = node->GetMesh(j);
            if (mesh == nullptr)
                continue;

            isSelected = (mesh == selEntity);
            if (ImGui::Selectable(("+" + depthMarker + mesh->GetName()).c_str(), &isSelected))
                selEntity = mesh;

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        // Add all child nodes to the list
        for (unsigned int i = 0; i < node->GetChildCount(); i++)
        {
            RenderNode(selEntity, node->GetChild(i), depth + 1);
        }
    }
public:
    void Draw() override
    {
        if (!mIsEnabled)
            return;

        if (ImGui::Begin("Hierarchy", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Node* root = mScene->GetRootNode();
            IEntity* selEntity = mState->GetSel()->GetSelectedEntity();
            if (ImGui::BeginListBox("##HierarchyRoot"))
            {
                RenderNode(selEntity, root, 0);
                if (mState->GetSel()->GetSelectedEntity() != selEntity)
                    mState->GetSel()->SelectEntity(selEntity);
                ImGui::EndListBox();
            }
        }
        ImGui::End();
    }

    GUIHierarchyWindow(State* state, Scene* scene, bool isEnabled)
    {
        mType = GUI::HIERARCHY;
        mState = state;
        mScene = scene;
        mIsEnabled = isEnabled;
    }
};