#include "window.h"

// Opens a OpenGL window with the given name
// -----------------------------------------
Window::Window(int width, int height, std::string name)
{
    // Set class variables
    // ---------------------------
    mWidth = width;
    mHeight = height;
    mName = name;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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
    images[0].pixels = stbi_load(FileSystem::GetPath("icon.png").c_str(), &images[0].width, &images[0].height, 0, 4);
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
    tempIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    mIO = &tempIO;
}

// Draws all GUIs
void Window::DrawGUI()
{
    // Start the Dear ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    for (auto iter = mGUIList.begin(); iter != mGUIList.end(); ++iter)
        if (iter->second->IsEnabled())
            iter->second->Draw();
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
        glEnable(GL_CULL_FACE);

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}