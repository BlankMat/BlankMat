#pragma once
#include "interfaces/iGUIWindow.h"
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
        std::string name = (mState->collapseScope) ? curEntity->GetUnscopedName() : curEntity->GetScopedName();
        curEntity->Enable(GUIWindowUtils::Checkbox("##entity" + curEntity->GetScopedName(), curEntity->IsEnabled()));
        ImGui::SameLine();

        GUIWindowUtils::Deselectable(depthMarker + name + "##" + curEntity->GetScopedName(), selEntity, curEntity);
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

        // If the node only holds a single mesh, render the mesh instead of the node
        if (mState->collapseMeshNodes && node->ChildCount() == 0 && node->MeshCount() == 1)
        {
            RenderSelectable(selEntity, node->GetMesh(0), depthMarker + "+  ");
            return;
        }

        // Mark this node
        bool isSelected = (IEntity::GetNameNullSafe(node) == IEntity::GetNameNullSafe(selEntity));
        bool isExpanded = mExpandedNodes.contains(node) || mState->expandAllNodes;
        const char* nodeMarker = isExpanded ? "v  " : ">  ";
        RenderSelectable(selEntity, node, depthMarker + nodeMarker);

        // Toggle node expanded status on right click
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            if (isExpanded)
                mExpandedNodes.erase(node);
            else
                mExpandedNodes.insert(node);
            isExpanded = mExpandedNodes.contains(node);
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

    void RenderNodeBase(IEntity*& selEntity, Node* node)
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

        if (ImGui::Begin("Hierarchy", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Options"))
                {
                    if (ImGui::MenuItem("Collapse Mesh Nodes"))
                        mState->ToggleCollapseMeshNodes();
                    if (ImGui::MenuItem("Collapse Scope"))
                        mState->ToggleCollapseScope();
                    if (ImGui::MenuItem("Expand All"))
                        mState->ToggleExpandAllNodes();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            Node* root = mScene->GetRootNode();
            IEntity* selEntity = mState->GetSel()->GetSelectedEntity();
            if (ImGui::BeginListBox("##HierarchyRoot"))
            {
                IEntity* prevSel = selEntity;
                RenderNodeBase(selEntity, root);
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