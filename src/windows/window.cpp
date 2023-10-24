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

// Draws all GUIs
void Window::DrawGUI()
{
    // Start the Dear ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

     // Check if the docking system is enabled
    if (mIO->ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        // Create a docking space
        if (!DockSpaceInitialized)
        {
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("DockSpace", nullptr, dockspace_flags);
            ImGui::PopStyleVar(3);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

            DockSpaceInitialized = true;
            ImGui::End();
        }
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