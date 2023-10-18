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

    // Render one row of the hierarchy
    void RenderSelectable(IEntity*& selEntity, IEntity* curEntity, std::string depthMarker)
    {
        // Add checkbox for enabling or disabling elements
        bool isEnabled = curEntity->IsEnabled();
        ImGui::Checkbox(("##entity" + curEntity->GetName()).c_str(), &isEnabled);
        curEntity->Enable(isEnabled);
        ImGui::SameLine();

        bool isSelected = (curEntity == selEntity);
        bool meshOpen = ImGui::Selectable((depthMarker + curEntity->GetName()).c_str(), &isSelected);
        // If the item is clicked, toggle selection
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            IEntity* newSelect = (selEntity != curEntity) ? curEntity : nullptr;
            std::cout << "Clicked " << IEntity::GetNameNullSafe(curEntity) << ". Changing from " << IEntity::GetNameNullSafe(selEntity)
                << " to " << IEntity::GetNameNullSafe(newSelect) << std::endl;
            selEntity = newSelect;
            isSelected = newSelect != nullptr;
        }

        // If the item is selected, highlight it
        if (isSelected)
            ImGui::SetItemDefaultFocus();
    }

    // Recursively renders the node and its children
    void RenderNode(IEntity*& selEntity, Node* node, int depth)
    {
        // Don't render null nodes
        if (node == nullptr)
            return;

        // Mark depth of nodes
        std::string depthMarker = "";
        for (int k = 0; k < depth - 1; k++)
            depthMarker += "  ";

        // Mark this node
        bool isSelected = (IEntity::GetNameNullSafe(node) == IEntity::GetNameNullSafe(selEntity));
        bool isExpanded = (mExpandedNodes.find(node) != mExpandedNodes.end());
        char* nodeMarker = isExpanded ? "v  " : ">  ";
        RenderSelectable(selEntity, node, depthMarker + nodeMarker);

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
            RenderSelectable(selEntity, mesh, depthMarker + "  +  ");
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
                IEntity* prevSel = selEntity;
                RenderNode(selEntity, root, 0);
                if (prevSel != selEntity)
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