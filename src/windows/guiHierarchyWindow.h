#pragma once
#include "iGUIWindow.h"
#include "guiWindowUtils.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include <set>

class GUIHierarchyWindow : public IGUIWindow
{
protected:
    State* mState = nullptr;
    Scene* mScene = nullptr;
    std::set<IEntity*> mExpandedNodes;

    // Render one row of the hierarchy
    void RenderSelectable(IEntity*& selEntity, IEntity* curEntity, const std::string& depthMarker)
    {
        // Add checkbox for enabling or disabling elements
        curEntity->Enable(
            GUIWindowUtils::Checkbox("##entity" + curEntity->GetName(), curEntity->IsEnabled()));
        ImGui::SameLine();

        GUIWindowUtils::Deselectable(depthMarker + curEntity->GetName(), selEntity, curEntity);
    }

    // Recursively renders the node and its children
    void RenderNode(IEntity*& selEntity, Node* node, int depth)
    {
        // Don't render null nodes
        if (node == nullptr)
            return;

        // Mark depth of nodes
        std::string depthMarker = "";
        for (int k = 0; k < depth; k++)
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
        for (unsigned int j = 0; j < node->MeshCount(); j++)
        {
            // Get mesh
            IEntity* mesh = node->GetMesh(j);
            if (mesh == nullptr)
                continue;
            RenderSelectable(selEntity, mesh, depthMarker + "  +  ");
        }

        // Render all child nodes if requested
        for (unsigned int i = 0; i < node->ChildCount(); i++)
        {
            RenderNode(selEntity, node->GetChild(i), depth + 1);
        }
    }

    void RenderNodeBase(IEntity* selEntity, Node* node)
    {
        // Render all meshes of the node
        for (unsigned int j = 0; j < node->MeshCount(); j++)
        {
            // Get mesh
            IEntity* mesh = node->GetMesh(j);
            if (mesh == nullptr)
                continue;
            RenderSelectable(selEntity, mesh, "+  ");
        }
        // Render all child nodes of the node
        for (unsigned int i = 0; i < node->ChildCount(); i++)
            RenderNode(selEntity, node->GetChild(i), 0);
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
                RenderNodeBase(prevSel, root);
                if (prevSel != selEntity)
                    mState->GetSel()->SelectEntity(selEntity);
                ImGui::EndListBox();
            }
        }
        ImGui::End();
    }

    GUIHierarchyWindow(State* state, Scene* scene, bool isEnabled)
        : mState(state), mScene(scene)
    {
        mType = GUI::HIERARCHY;
        mIsEnabled = isEnabled;
    }
};