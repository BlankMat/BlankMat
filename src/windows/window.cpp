#include "window.h"
#include "imgui.h"
#include "imgui_internal.h" // Include this header for ImGuiDockBuilder

// Define a flag to check if the docking space is initialized
bool DockSpaceInitialized = false;

// Opens a OpenGL window with the given name
// -----------------------------------------
Window::Window(int width, int height, std::string name, Config* config, State* state)
{
    // Set class variables
    // ---------------------------
    mWidth = width;
    mHeight = height;
    mName = name;
    Config* styleConfig = config->GetConfig("style");
    Config* qualityConfig = config->GetConfig("quality");

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    const char* windowName = mName.c_str();
    mWindow = glfwCreateWindow(mWidth, mHeight, windowName, NULL, NULL);
    if (mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set icon
    GLFWimage images[1];
    images[0].pixels = stbi_load(FileSystem::GetPath(ICON).c_str(), &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(mWindow, 1, images);
    stbi_image_free(images[0].pixels);

    stbi_set_flip_vertically_on_load(true);

    // // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    CalcWindowSize();

    // Dear imGUI
    // -----------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& tempIO = ImGui::GetIO();
    tempIO.Fonts->AddFontFromFileTTF(FileSystem::GetPath(FONT_DIR + styleConfig->GetString("font")).c_str(), styleConfig->GetFloat("fontSize"));
    tempIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    mIO = &tempIO;
    SetupImGuiStyle(styleConfig->GetBool("darkTheme"), styleConfig->GetFloat("windowOpacity"));

    // Dear ImGui Docking
    DockSpaceInitialized = false;
    mIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking

    // Quality Settings
    // ----------------------------
    unsigned int shadowWidth = state->depthMapSize;
    unsigned int shadowHeight = shadowWidth;

    // Generate depth map frame buffer
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // Create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0 , GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Attach depth texture to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    state->depthMapFBO = depthMapFBO;
    state->depthMap = depthMap;
}

// Sets up the ImGui Style
inline void SetupImGuiStyle(bool isDarkStyle, float alphaThreshold)
{
    //Use a ternary operator
    isDarkStyle ? ImGui::StyleColorsDark() : ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();

    // Adjusts the alpha values of the ImGui colors based on the alpha threshold.
    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {
        const auto color_id = static_cast<ImGuiCol>(i);
        auto& color = style.Colors[i];
        if (color.w < alphaThreshold || color_id == ImGuiCol_FrameBg || color_id == ImGuiCol_WindowBg || color_id == ImGuiCol_ChildBg)
        {
            color.w *= alphaThreshold;
        }
    }

    // Sets the border sizes and rounding.
    style.ChildBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameRounding = 3.0f;
    style.Alpha = 1.0f;
}

void ShowExampleAppDockSpace(bool* p_open)
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                *p_open = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void Window::SetupDefaultDockingLayout()
{
    if (DockSpaceInitialized) // To ensure this runs once
        return;

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

    // Split the dockspace into left, right and bottom
    ImGuiID dock_main = dockspace_id;
    ImGuiID dock_left = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Left, 0.25f, NULL, &dock_main);
    ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right, 0.25f, NULL, &dock_main);
    ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Down, 0.25f, NULL, &dock_main);

    // Now dock the windows in the desired locations.
    ImGui::DockBuilderDockWindow("Debug Tools", dock_left);
    ImGui::DockBuilderDockWindow("Light Viewer", dock_right);
    ImGui::DockBuilderDockWindow("Hierarchy", dock_bottom);
    ImGui::DockBuilderDockWindow("Material Viewer", dock_bottom);
    ImGui::DockBuilderDockWindow("Material Editor", dock_bottom);
    ImGui::DockBuilderDockWindow("Inspector", dock_bottom);

    ImGui::DockBuilderFinish(dockspace_id);

    DockSpaceInitialized = true;
}

// Draws all GUIs
void Window::DrawGUI()
{
    // Start the Dear ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set up DockSpace every frame
    ShowExampleAppDockSpace(&DockSpaceInitialized);

    // Draw each GUI
    for (auto& pair : mGUIList) {
        IGUIWindow* guiWindow = pair.second;
        GUI guiType = pair.first; // Assuming first element of pair is the identifier

        switch (guiType) {
        case GUI::DEBUG_TOOLS: // Docking to the left of the screen
            ImGui::SetNextWindowPos(ImVec2(0, 48)); // 20 for menu bar height; adjust as needed
            ImGui::SetNextWindowSize(ImVec2((float)mWidth * 0.2f, (float)mHeight));
            break;
        case GUI::LIGHT_VIEWER: // Docking to the right of the screen
            ImGui::SetNextWindowPos(ImVec2((float)mWidth * 0.8f, 48));
            ImGui::SetNextWindowSize(ImVec2((float)mWidth * 0.2f, (float)mHeight));
            break;
        default: // All other windows docked to the bottom of the screen
            ImGui::SetNextWindowPos(ImVec2((float)mWidth * 0.2f, (float)mHeight * 0.8f)); // Start right after Debug Tools
            ImGui::SetNextWindowSize(ImVec2((float)mWidth * 0.6f, (float)mHeight * 0.8f - 48)); // End right before Light Viewer
            break;
        }

        guiWindow->Draw();
    }

    if (!DockSpaceInitialized) {
        DockSpaceInitialized = true;
    }

    for (auto iter = mGUIList.begin(); iter != mGUIList.end(); ++iter)
        if (iter->second->IsEnabled())
            iter->second->Draw();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Adds the given GUI window
void Window::AddGUI(IGUIWindow* gui)
{
    if (mGUIList.find(gui->GetType()) != mGUIList.end())
    {
        std::cout << "ERROR::WINDOW::EXISTS GUI with type " << gui->GetName() << " already exists." << std::endl;
        return;
    }
    mGUIList.emplace(gui->GetType(), gui);
}

// Gets the GUI with the given type
IGUIWindow* Window::GetGUI(GUI type)
{
    if (mGUIList.find(type) == mGUIList.end())
    {
        std::cout << "ERROR::WINDOW::NULL GUI with type " << (int)type << " is not open." << std::endl;
        return nullptr;
    }
    return mGUIList[type];
}

// Enable or disable wireframe
// ------------------------------------------
void OpenGLEnableWireframe(bool enable)
{
    if (enable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Disable culling
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // Enable culling
        glDisable(GL_CULL_FACE);

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

// Initialize GUI
//void Window::InitGUI(State* state, Scene* scene)
//{
//
//}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}