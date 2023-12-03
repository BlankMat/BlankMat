#pragma once
#include "interfaces/iGUIWindow.h"
#include "interaction/input.h"
#include <unordered_map>

class GUIMenuBarWindow : public IGUIWindow
{
protected:
    Input* mInput = nullptr;
    std::unordered_map<std::string, std::string> mHotkeys;

    // Shows an example menu file. Replace with real items later
    void ShowFileMenu()
    {
        if (ImGui::MenuItem("New Scene##File", GetHotkey("NEW_SCENE")))
            mInput->RunCommand("NEW_SCENE");
        if (ImGui::MenuItem("Open Scene##File", GetHotkey("OPEN_SCENE")))
            mInput->RunCommand("OPEN_SCENE");
        if (ImGui::MenuItem("Save Scene##File", GetHotkey("SAVE_SCENE")))
            mInput->RunCommand("SAVE_SCENE");
        if (ImGui::MenuItem("Save Scene As...##File", GetHotkey("SAVE_SCENE_AS")))
            mInput->RunCommand("SAVE_SCENE_AS");
        if (ImGui::MenuItem("Save and Increment##File", GetHotkey("SAVE_SCENE_INCREMENT")))
            mInput->RunCommand("SAVE_SCENE_INCREMENT");

        ImGui::Separator();

        if (ImGui::MenuItem("Import##File", GetHotkey("IMPORT")))
            mInput->RunCommand("IMPORT");
        if (ImGui::MenuItem("Export##File", GetHotkey("EXPORT")))
            mInput->RunCommand("EXPORT");

        ImGui::Separator();

        if (ImGui::MenuItem("Quit##File", GetHotkey("QUIT")))
            mInput->RunCommand("QUIT");
    }

    void ShowEditMenu()
    {
        if (ImGui::MenuItem("Undo##Edit", GetHotkey("UNDO")))
            mInput->RunCommand("UNDO");
        if (ImGui::MenuItem("Redo##Edit", GetHotkey("REDO"), false, false))
            mInput->RunCommand("REDO");
        if (ImGui::MenuItem("Repeat##Edit", GetHotkey("REPEAT"), false, false))
            mInput->RunCommand("REPEAT");

        ImGui::Separator();
        
        if (ImGui::MenuItem("Cut##Edit", GetHotkey("CUT")))
            mInput->RunCommand("CUT");
        if (ImGui::MenuItem("Copy##Edit", GetHotkey("COPY")))
            mInput->RunCommand("COPY");
        if (ImGui::MenuItem("Paste##Edit", GetHotkey("PASTE")))
            mInput->RunCommand("PASTE");
    }

    void ShowViewMenu()
    {
        if (ImGui::BeginMenu("Debug##View"))
        {
            if (ImGui::MenuItem("Debug Tools##View", GetHotkey("OPEN_DEBUG_TOOLS")))
                mInput->RunCommand("OPEN_DEBUG_TOOLS");
            if (ImGui::MenuItem("Action List##View", GetHotkey("OPEN_ACTION_LIST")))
                mInput->RunCommand("OPEN_ACTION_LIST");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Rendering##View"))
        {
            if (ImGui::MenuItem("Material Editor##View", GetHotkey("OPEN_MATERIAL_EDITOR")))
                mInput->RunCommand("OPEN_MATERIAL_EDITOR");
            if (ImGui::MenuItem("Material Viewer##View", GetHotkey("OPEN_MATERIAL_VIEWER")))
                mInput->RunCommand("OPEN_MATERIAL_VIEWER");
            if (ImGui::MenuItem("Texture Viewer##View", GetHotkey("OPEN_TEXTURE_VIEWER")))
                mInput->RunCommand("OPEN_TEXTURE_VIEWER");
            if (ImGui::MenuItem("Light Viewer##View", GetHotkey("OPEN_LIGHT_VIEWER")))
                mInput->RunCommand("OPEN_LIGHT_VIEWER");
            if (ImGui::MenuItem("Camera Viewer##View", GetHotkey("OPEN_CAMERA_VIEWER")))
                mInput->RunCommand("OPEN_CAMERA_VIEWER");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene##View"))
        {
            if (ImGui::MenuItem("Hierarchy##View", GetHotkey("OPEN_HIERARCHY")))
                mInput->RunCommand("OPEN_HIERARCHY");
            if (ImGui::MenuItem("Inspector##View", GetHotkey("OPEN_INSPECTOR")))
                mInput->RunCommand("OPEN_INSPECTOR");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools##View"))
        {
            if (ImGui::MenuItem("Toolbar##View", GetHotkey("OPEN_TOOLBAR")))
                mInput->RunCommand("OPEN_TOOLBAR");
            if (ImGui::MenuItem("Tool Mode##View", GetHotkey("OPEN_TOOL_MODE")))
                mInput->RunCommand("OPEN_TOOL_MODE");
            ImGui::EndMenu();
        }
    }

    void ShowCreateMenu()
    {
        if (ImGui::MenuItem("New Cube##Create", GetHotkey("NEW_CUBE")))
            mInput->RunCommand("NEW_CUBE");
        if (ImGui::MenuItem("New Plane##Create", GetHotkey("NEW_PLANE")))
            mInput->RunCommand("NEW_PLANE");
        if (ImGui::MenuItem("New Sphere##Create", GetHotkey("NEW_SPHERE")))
            mInput->RunCommand("NEW_SPHERE");
    }

    void ShowRenderingMenu()
    {
        if (ImGui::MenuItem("Toggle Diffuse Maps##Rendering", GetHotkey("TOGGLE_DIFFUSE_MAP")))
            mInput->RunCommand("TOGGLE_DIFFUSE_MAP");
        if (ImGui::MenuItem("Toggle Ambient Maps##Rendering", GetHotkey("TOGGLE_AMBIENT_MAP")))
            mInput->RunCommand("TOGGLE_AMBIENT_MAP");
        if (ImGui::MenuItem("Toggle Specular Maps##Rendering", GetHotkey("TOGGLE_SPECULAR_MAP")))
            mInput->RunCommand("TOGGLE_SPECULAR_MAP");
        if (ImGui::MenuItem("Toggle Normal Maps##Rendering", GetHotkey("TOGGLE_NORMAL_MAP")))
            mInput->RunCommand("TOGGLE_NORMAL_MAP");
        if (ImGui::MenuItem("Toggle Height Maps##Rendering", GetHotkey("TOGGLE_HEIGHT_MAP")))
            mInput->RunCommand("TOGGLE_HEIGHT_MAP");
        if (ImGui::MenuItem("Toggle Alpha Maps##Rendering", GetHotkey("TOGGLE_ALPHA_MAP")))
            mInput->RunCommand("TOGGLE_ALPHA_MAP");

        ImGui::Separator();

        if (ImGui::MenuItem("Toggle Grid##Rendering", GetHotkey("TOGGLE_GRID")))
            mInput->RunCommand("TOGGLE_GRID");
        if (ImGui::MenuItem("Toggle Shadows##Rendering", GetHotkey("TOGGLE_SHADOWS")))
            mInput->RunCommand("TOGGLE_SHADOWS");
        if (ImGui::MenuItem("Toggle Default Material##Rendering", GetHotkey("TOGGLE_DEFAULT_MAT")))
            mInput->RunCommand("TOGGLE_DEFAULT_MAT");

        ImGui::Separator();

        if (ImGui::MenuItem("Flat Lighting##Rendering", GetHotkey("LIGHTING_SHADED")))
            mInput->RunCommand("LIGHTING_SHADED");
        if (ImGui::MenuItem("Textured Flat Lighting##Rendering", GetHotkey("LIGHTING_TEXTURED")))
            mInput->RunCommand("LIGHTING_TEXTURED");
        if (ImGui::MenuItem("Textured Lit Lighting##Rendering", GetHotkey("LIGHTING_LIGHTS")))
            mInput->RunCommand("LIGHTING_LIGHTS");
        if (ImGui::MenuItem("Wireframe Lighting##Rendering", GetHotkey("LIGHTING_WIREFRAME")))
            mInput->RunCommand("LIGHTING_WIREFRAME");

        ImGui::Separator();

        if (ImGui::MenuItem("Render##Rendering", GetHotkey("RENDER")))
            mInput->RunCommand("RENDER");
    }

    const char* GetHotkey(const std::string& command)
    {
        if (mHotkeys.contains(command))
            return mHotkeys[command].c_str();
        return nullptr;
    }

public:
	void Draw() override
	{
        if (!mIsEnabled)
            return;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File##MainMenu"))
            {
                ShowFileMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit##MainMenu"))
            {
                ShowEditMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View##MainMenu"))
            {
                ShowViewMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Create##MainMenu"))
            {
                ShowCreateMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Rendering##MainMenu"))
            {
                ShowRenderingMenu();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
	}

    GUIMenuBarWindow(Input* input, Config* config, bool isEnabled)
        : mInput(input)
    {
        mType = GUI::MENU_BAR;
        mIsEnabled = isEnabled;

        // Load hotkey configs, saving them in the order [command, hotkey]
        auto& hotkeys = config->GetConfig("hotkeys")->GetStrings();
        for (auto iter = hotkeys.begin(); iter != hotkeys.end(); ++iter)
            mHotkeys.emplace(iter->second, iter->first);
    }
};


/*
if (ImGui::BeginMenu("Open Recent"))
{
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    if (ImGui::BeginMenu("More.."))
    {
        ImGui::MenuItem("Hello");
        ImGui::MenuItem("Sailor");
        if (ImGui::BeginMenu("Recurse.."))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    ImGui::EndMenu();
}

        if (ImGui::BeginMenu("Options"))
        {
            static bool enabled = true;
            ImGui::MenuItem("Enabled", "", &enabled);
            ImGui::BeginChild("child", ImVec2(0, 60), true);
            for (int i = 0; i < 10; i++)
                ImGui::Text("Scrolling Text %d", i);
            ImGui::EndChild();
            static float f = 0.5f;
            static int n = 0;
            ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
            ImGui::InputFloat("Input", &f, 0.1f);
            ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Colors"))
        {
            float sz = ImGui::GetTextLineHeight();
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                ImGui::Dummy(ImVec2(sz, sz));
                ImGui::SameLine();
                ImGui::MenuItem(name);
            }
            ImGui::EndMenu();
        }
*/