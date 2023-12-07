#include "window.h"

// Opens a OpenGL window with the given name
// -----------------------------------------
Window::Window(int width, int height, const std::string& name, Config* config)
    : mWidth(width), mHeight(height), mName(name), mConfig(config)
{
    // State must be initialized first, since it is needed for other init
    mState = new State(config);

    // Setup all components of the window, returning if any of them fail
    // GLFW must be setup first
    if (!SetupGLFW())
        return;
    // Input must be setup before ImGui
    if (!SetupInput())
        return;
    // Icon must be setup after window is created
    if (!SetupIcon())
        return;
    // GLAD must be setup after GLFW
    if (!SetupGLAD())
        return;
    // ImGui must be setup after GLFW and GLAD
    if (!SetupImGui(config))
        return;
    // Setup shadows must be setup after GLFW and GLAD
    if (!SetupShadows())
        return;

    // Initialize scene last, since it it needs GLFW and GLAD to already be init
    mScene = new Scene(mState);

    // Once the window is setup, initialize IO and file operations
    mInput = new Input(mIO, mState, config);
    mSceneIO = new SceneIO(mState, mScene, mWindow);

    // After IO is setup, load GUIs
    LoadGUIs();
}

// Draws all GUIs
void Window::DrawGUI(State* state)
{
    // Start the Dear ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Check if the docking system is enabled
    if (mIO->ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        // Create a docking space
        if (!mIsDockSpaceInitialized)
        {
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Dockspace", nullptr, dockspace_flags);
            ImGui::PopStyleVar(3);

            ImGuiID dockspace_id = ImGui::GetID("Dockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

            mIsDockSpaceInitialized = true;
            ImGui::End();
        }
    }

    if (state->drawGUI)
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
    if (IsGUIOpen(gui->GetType()))
    {
        std::cout << "ERROR::WINDOW::EXISTS GUI with type " << gui->GetName() << " already exists." << std::endl;
        return;
    }
    mGUIList.emplace(gui->GetType(), gui);
}

// Gets the GUI with the given type
IGUIWindow* Window::GetGUI(GUI type)
{
    if (!IsGUIOpen(type))
    {
        std::cout << "ERROR::WINDOW::NULL GUI with type " << (int)type << " is not open." << std::endl;
        return nullptr;
    }
    return mGUIList[type];
}

// Returns whether the given type of GUI is open
bool Window::IsGUIOpen(GUI type)
{
    return mGUIList.contains(type);
}

// Opens the given GUI if it is not open yet
void Window::OpenGUI(GUI gui)
{
    // If the GUI already exists, just enable it
    if (IsGUIOpen(gui))
    {
        IGUIWindow* guiWindow = GetGUI(gui);
        if (guiWindow != nullptr)
            guiWindow->Enable();
        return;
    }

    // If the GUI does not exist, add it
    switch (gui)
    {
    case GUI::DEBUG_TOOLS:
        AddGUI(new GUIDebugToolsWindow(mState, mScene, true));
        return;
    case GUI::HIERARCHY:
        AddGUI(new GUIHierarchyWindow(mState, mScene, true));
        return;
    case GUI::INSPECTOR:
        AddGUI(new GUIInspectorWindow(mState, mScene, true));
        return;
    case GUI::MENU_BAR:
        AddGUI(new GUIMenuBarWindow(mInput, mConfig, true));
        return;
    case GUI::TOOLBAR:
        AddGUI(new GUIToolbarWindow(mState, mScene, true));
        return;
    case GUI::MATERIAL_BAR:
        return;
    case GUI::TRANSFORM_EDITOR:
        return;
    case GUI::MODEBAR:
        AddGUI(new GUIToolModeWindow(mState, mScene, true));
        return;
    case GUI::MODEL_EDITOR:
        return;
    case GUI::UV_EDITOR:
        return;
    case GUI::MATERIAL_VIEWER:
        AddGUI(new GUIMaterialViewer(mState, mScene, true));
        return;
    case GUI::TEXTURE_VIEWER:
        AddGUI(new GUITextureViewer(mState, mScene, true));
        return;
    case GUI::LIGHT_VIEWER:
        AddGUI(new GUILightViewer(mState, mScene, true));
        return;
    case GUI::CAMERA_VIEWER:
        AddGUI(new GUICameraViewer(mState, mScene, true));
        return;
    case GUI::ACTION_LIST:
        AddGUI(new GUIActionList(mState, mScene, true));
        return;
    case GUI::NONE:
    default:
        return;
    }
}

// Opens all defined GUIs
void Window::LoadGUIs()
{
    OpenGUI(GUI::DEBUG_TOOLS);
    OpenGUI(GUI::HIERARCHY);
    OpenGUI(GUI::INSPECTOR);
    OpenGUI(GUI::MENU_BAR);
    OpenGUI(GUI::TOOLBAR);
    OpenGUI(GUI::MATERIAL_BAR);
    OpenGUI(GUI::TRANSFORM_EDITOR);
    OpenGUI(GUI::MODEBAR);
    OpenGUI(GUI::MODEL_EDITOR);
    OpenGUI(GUI::MATERIAL_EDITOR);
    OpenGUI(GUI::TEXTURE_EDITOR);
    OpenGUI(GUI::UV_EDITOR);
    OpenGUI(GUI::MATERIAL_VIEWER);
    OpenGUI(GUI::TEXTURE_VIEWER);
    OpenGUI(GUI::LIGHT_VIEWER);
    OpenGUI(GUI::CAMERA_VIEWER);
    OpenGUI(GUI::ACTION_LIST);
}

// Updates the window title to match the current state of the application
void Window::UpdateWindowTitle(const std::string& fileName, bool isSaved)
{
    std::string name = mName + " - ";
    name += (fileName != "") ? fileName : "Untitled Scene";
    if (!isSaved)
        name += "*";
    glfwSetWindowTitle(mWindow, name.c_str());
}

// Sets up GLFW for the app
bool Window::SetupGLFW()
{
    // Initialize GLFW to proper version
    glfwInit();
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    const char* windowName = mName.c_str();
    mWindow = glfwCreateWindow(mWidth, mHeight, windowName, NULL, NULL);
    if (mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, Input::FramebufferSizeCallback);

    // Return success
    return true;
}

// Sets up GLAD for the app
bool Window::SetupGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Return success
    return true;
}

// Sets up input for the app
bool Window::SetupInput()
{
    // Setup window to be able to use the input module
    glfwSetWindowUserPointer(mWindow, this);

    glfwSetKeyCallback(mWindow, KeyCallback);
    glfwSetCursorPosCallback(mWindow, CursorPositionCallback);
    glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
    glfwSetScrollCallback(mWindow, ScrollCallback);
    Input::ShowMouseCursor(mWindow);
    return true;
}

// Sets the icon for the app
bool Window::SetupIcon()
{
    stbi_set_flip_vertically_on_load(false);
    GLFWimage images[1]{};
    images[0].pixels = stbi_load(FileSystem::GetPath(ICON).c_str(), &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(mWindow, 1, images);
    stbi_image_free(images[0].pixels);
    stbi_set_flip_vertically_on_load(mState->flipTextures);
    return true;
}

// Sets up ImGui for the app
bool Window::SetupImGui(Config* config)
{
    Config* styleConfig = config->GetConfig("style");

    // Create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    mIO = &ImGui::GetIO();
    (void)mIO;

    // Handle font
    mIO->Fonts->AddFontFromFileTTF(FileSystem::GetPath(FONT_DIR + styleConfig->GetString("font")).c_str(), styleConfig->GetFloat("fontSize"));
    mIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Dear ImGui Docking
    mIsDockSpaceInitialized = false;
    mIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup style
    SetupImGuiStyle(styleConfig->GetBool("darkTheme"), styleConfig->GetFloat("windowOpacity"));

    // Setup ImGui Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Return success
    return true;
}

// Sets up the ImGui Style
bool Window::SetupImGuiStyle(bool isDarkStyle, float alphaThreshold)
{
    // Enable the correct style colors
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
    style.WindowBorderSize = 1.0f;
    style.FrameRounding = 3.0f;
    style.Alpha = 1.0f;

    // Return success
    return true;
}

// Sets up shadows for the app
bool Window::SetupShadows()
{
    // Quality Settings
    unsigned int shadowWidth = mState->depthMapSize;
    unsigned int shadowHeight = shadowWidth;

    // Generate depth map frame buffer
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // Create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach depth texture to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    mState->depthMapFBO = depthMapFBO;
    mState->depthMap = depthMap;

    // Return success
    return true;
}