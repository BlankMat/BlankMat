#pragma once
#include "iGUIWindow.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include <set>

class GUIHierarchyWindow : public IGUIWindow
{
protected:
    State* mState;
    Scene* mScene;
    std::set<IEntity*> mExpandedNodes;

    // Recursively renders the node and its children
    void RenderNode(IEntity*& selEntity, Node* node, int depth)
    {
        // Mark depth of nodes
        std::string depthMarker = "";
        for (int k = 0; k < depth - 1; k++)
            depthMarker += "  ";

        // Mark this node
        bool isSelected = (node == selEntity);
        bool isExpanded = (mExpandedNodes.find(node) != mExpandedNodes.end());
        char* nodeMarker = isExpanded ? "v  " : ">  ";
        bool nodeOpen = ImGui::Selectable((nodeMarker + depthMarker + node->GetName()).c_str(), &isSelected);

        // If the element is clicked, toggle selection
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            std::cout << "Clicked " << ((node != nullptr) ? node->GetName() : "None") << ". Changing from " <<
                ((selEntity != nullptr) ? selEntity->GetName() : "") << " to " << ((selEntity != node) ? node->GetName() : "null") << std::endl;
            selEntity = (selEntity != node) ? node : nullptr;
            isSelected = (node == selEntity);
        }

        // If the item is selected, highlight it
        if (isSelected)
            ImGui::SetItemDefaultFocus();

        // Toggle node expanded status on right click
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            if (isExpanded)
                mExpandedNodes.erase(node);
            else
                mExpandedNodes.insert(node);
            isExpanded = (mExpandedNodes.find(node) != mExpandedNodes.end());
        }

        // If the node is not expanded, skip it
        if (!isExpanded)
            return;

        // Render all meshes of node
        for (unsigned int j = 0; j < node->GetMeshCount(); j++)
        {
            // Get mesh
            IEntity* mesh = node->GetMesh(j);
            if (mesh == nullptr)
                continue;

            isSelected = (mesh == selEntity);
            bool meshOpen = ImGui::Selectable(("+    " + depthMarker + mesh->GetName()).c_str(), &isSelected);
            // If the item is clicked, toggle selection
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                selEntity = (selEntity != mesh) ? mesh : nullptr;
                isSelected = (mesh == selEntity);
            }

            // If the item is selected, highlight it
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        // Render all child nodes if requested
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