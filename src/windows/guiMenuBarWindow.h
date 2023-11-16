#pragma once
#include "iGUIWindow.h"
#include <unordered_map>

class GUIMenuBarWindow : public IGUIWindow
{
protected:
    std::unordered_map<std::string, std::string> mHotkeys;

    // Shows an example menu file. Replace with real items later
    void ShowFileMenu()
    {
        if (ImGui::MenuItem("New Scene", GetHotkey("NEW_SCENE"))) {}
        if (ImGui::MenuItem("Open Scene", GetHotkey("OPEN_SCENE"))) {}
        if (ImGui::MenuItem("Save Scene", GetHotkey("SAVE_SCENE"))) {}
        if (ImGui::MenuItem("Save Scene As...", GetHotkey("SAVE_SCENE_AS"))) {}
        if (ImGui::MenuItem("Save and Increment", GetHotkey("SAVE_SCENE_INCREMENT"))) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Import", GetHotkey("IMPORT"))) {}
        if (ImGui::MenuItem("Export", GetHotkey("EXPORT"))) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", GetHotkey("QUIT"))) {}
    }

    void ShowEditMenu()
    {
        if (ImGui::MenuItem("Undo", GetHotkey("UNDO"))) {}
        if (ImGui::MenuItem("Redo", GetHotkey("REDO"), false, false)) {}
        if (ImGui::MenuItem("Repeat", GetHotkey("REPEAT"), false, false)) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", GetHotkey("CUT"))) {}
        if (ImGui::MenuItem("Copy", GetHotkey("COPY"))) {}
        if (ImGui::MenuItem("Paste", GetHotkey("PASTE"))) {}
    }

    void ShowViewMenu()
    {
        if (ImGui::BeginMenu("Debug"))
        {
            if (ImGui::MenuItem("Debug Tools")) {}
            if (ImGui::MenuItem("Action List")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Rendering"))
        {
            if (ImGui::MenuItem("Material Viewer")) {}
            if (ImGui::MenuItem("Material Editor")) {}
            if (ImGui::MenuItem("Light Viewer")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("Hierarchy")) {}
            if (ImGui::MenuItem("Inspector")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Toolbar")) {}
            if (ImGui::MenuItem("Tool Mode")) {}
            ImGui::EndMenu();
        }
    }

    void ShowCreateMenu()
    {
        if (ImGui::MenuItem("New Cube", GetHotkey("NEW_CUBE"))) {}
        if (ImGui::MenuItem("New Plane", GetHotkey("NEW_PLANE"))) {}
        if (ImGui::MenuItem("New Sphere", GetHotkey("NEW_SPHERE"))) {}
    }

    void ShowRenderingMenu()
    {
        if (ImGui::MenuItem("Toggle Diffuse Maps", GetHotkey("TOGGLE_DIFFUSE_MAP"))) {}
        if (ImGui::MenuItem("Toggle Ambient Maps", GetHotkey("TOGGLE_AMBIENT_MAP"))) {}
        if (ImGui::MenuItem("Toggle Specular Maps", GetHotkey("TOGGLE_SPECULAR_MAP"))) {}
        if (ImGui::MenuItem("Toggle Normal Maps", GetHotkey("TOGGLE_NORMAL_MAP"))) {}
        if (ImGui::MenuItem("Toggle Height Maps", GetHotkey("TOGGLE_HEIGHT_MAP"))) {}
        if (ImGui::MenuItem("Toggle Alpha Maps", GetHotkey("TOGGLE_ALPHA_MAP"))) {}
        if (ImGui::MenuItem("Toggle Grid", GetHotkey("TOGGLE_GRID"))) {}
        if (ImGui::MenuItem("Toggle Shadows", GetHotkey("TOGGLE_SHADOWS"))) {}
        if (ImGui::MenuItem("Toggle Default Material", GetHotkey("TOGGLE_DEFAULT_MAT"))) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Flat Lighting", GetHotkey("LIGHTING_SHADED"))) {}
        if (ImGui::MenuItem("Textured Flat Lighting", GetHotkey("LIGHTING_TEXTURED"))) {}
        if (ImGui::MenuItem("Textured Lit Lighting", GetHotkey("LIGHTING_LIGHTS"))) {}
        if (ImGui::MenuItem("Wireframe Lighting", GetHotkey("LIGHTING_WIREFRAME"))) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Render", GetHotkey("RENDER"))) {}
    }

    const char* GetHotkey(const std::string& command)
    {
        if (mHotkeys.find(command) != mHotkeys.end())
            return mHotkeys[command].c_str();
        return "";
    }

public:
	void Draw() override
	{
        if (!mIsEnabled)
            return;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ShowFileMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ShowEditMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ShowViewMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Create"))
            {
                ShowCreateMenu();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Rendering"))
            {
                ShowRenderingMenu();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
	}

    GUIMenuBarWindow(Config* config, bool isEnabled)
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