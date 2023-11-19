#include "main.h"

int main()
{
    // Read Configs
    Config* config = ConfigReader::ReadFile(FileSystem::GetPath(CONFIG_JSON));
    config->SetConfig("hotkeys", ConfigReader::ReadFile(FileSystem::GetPath(HOTKEYS_JSON)));
    config->SetConfig("materials", ConfigReader::ReadFile(FileSystem::GetPath(MATS_JSON)));
    config->SetConfig("shaders", ConfigReader::ReadFile(FileSystem::GetPath(SHADERS_JSON)));

    // Init window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, APP_NAME, config);
    if (window == nullptr)
        return -1;

    // Initialize commands for usage
    State* state = window->GetState();
    Scene* scene = window->GetScene();
    Commands::InitializeCommands(window);

    // Create scene
    scene->LoadMaterials(config->GetConfig("materials"));
    SceneReader::LoadScene(scene, FileSystem::GetPath(MODELS_DIR) + config->GetString("model.file"), 
        config->GetVec("model.pos"), config->GetVec("model.rot"), config->GetVec("model.scale"));

    state->GetSel()->SetTransformHandle(new PHandle("transformHandle", 0.5f, 6, true, glm::vec3(0.0f)));
    scene->SetTransformHandle(state->GetSel()->GetTransformHandle());

    // Load shaders
    LoadShaders(scene, config->GetConfig("shaders"));
    state->defaultMat = scene->SetDefaultMaterial("default");

    // Load default scene
    LoadDefaultScene(scene, state, state->defaultMat, config->GetBool("defaultCubes"), config->GetConfig("camera"), config->GetConfig("light"));
    window->GetInput()->SetMouseRotFunction(std::bind(&Camera::Rotate, scene->GetCamera(), std::placeholders::_1));
    window->GetInput()->SetMouseMoveFunction(std::bind(&Camera::Translate, scene->GetCamera(), std::placeholders::_1));
    window->GetInput()->SetMouseZoomFunction(std::bind(&Camera::Translate, scene->GetCamera(), std::placeholders::_1));

    // Add GUIs
    LoadGUIs(window, state, scene, window->GetInput(), config);

    // Track time
    double prevFrameTime = glfwGetTime();
    double curFrameTime = glfwGetTime();
    double lastSecond = glfwGetTime();
    int numFrames = 0;

    // render loop
    // -----------
    while (true)
    {
        // Poll events at the start of the loop
        glfwPollEvents();

        // If the window should close, handle exiting
        if (HandleWindowQuit(window, state))
            break;

        // Get deltaTime
        prevFrameTime = curFrameTime;
        curFrameTime = glfwGetTime();
        state->deltaTime = float(curFrameTime - prevFrameTime);
        CalculateFPS(state, lastSecond, numFrames);

        // Render
        OpenGLDraw(window, state, scene);

        // Draw GUI
        if (state->drawGUI)
            window->DrawGUI();

        // Swap buffers at the end of the loop
        glfwSwapBuffers(window->GetWindow());
        window->UpdateWindowTitle(state->GetCurFileName(), state->IsSaved());
    }

    // Clear up dynamic memory usage
    delete scene;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete window;
    return 0;
}

// Draws the current scene
// --------------------------------------------------------
void OpenGLDraw(Window* window, State* state, Scene* scene)
{
    // Update light
    scene->GetLight()->UpdateRotatingLight(state->isRotatingLight);
    scene->GetLight()->UpdateDiscoLight(state->isDiscoLight);
    
    // Clear BG
    glm::vec3 bgColor = scene->GetCamera()->GetBGColor();
    glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);

    // Render shadows
    if (state->enableShadows)
    {
        // Clear viewport
        glViewport(0, 0, state->depthMapSize, state->depthMapSize);
        glBindFramebuffer(GL_FRAMEBUFFER, state->depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Render scene shadows
        scene->DrawShadows(window, scene->GetShader("shadowMap"));
    }

    // Reset viewport
    glViewport(0, 0, window->GetWidth(), window->GetHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the scene
    scene->Draw(window, scene->GetShader(scene->GetCurShader()));

    // Clear materials loaded for this frame
    state->ClearLoadedMaterials();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Opens all defined GUIs
// ------------------------------------------------------
void LoadGUIs(Window* window, State* state, Scene* scene, Input* input, Config* config)
{
    window->AddGUI(new GUILightViewer(state, scene, true));
    window->AddGUI(new GUIMaterialViewer(state, scene, true));
    window->AddGUI(new GUIMaterialEditor(state, scene, true));
    window->AddGUI(new GUIDebugToolsWindow(state, scene, true));
    window->AddGUI(new GUIHierarchyWindow(state, scene, true));
    window->AddGUI(new GUIInspectorWindow(state, scene, true));
    window->AddGUI(new GUIMenuBarWindow(input, config, true));
    window->AddGUI(new GUIToolbarWindow(state, scene, true));
    window->AddGUI(new GUIToolModeWindow(state, scene, true));
    window->AddGUI(new GUIActionList(state, scene, true));
}

// Loads all defined shaders
// --------------------------------------------------
void LoadShaders(Scene* scene, Config* shaderConfig)
{
    double startTime = glfwGetTime();
    std::unordered_map<std::string, Config*> shaders = shaderConfig->GetConfigs();
    for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
    {
        scene->CreateShader(iter->first, iter->second);
    }
    scene->UseShader("default");
    Timer::Time(startTime, "Shaders loaded");
}

// Loads the default scene
// --------------------------------------------------------------------------------------------------------------------
void LoadDefaultScene(Scene* scene, State* state, Material* defaultMat, bool defaultCubes, Config* cameraConfig, Config* lightConfig)
{
    // Add light and camera
    scene->SetCamera(state->GetActionStack(), cameraConfig);
    scene->SetLight(new PLightCube("globalLight", 1.0f, lightConfig));

    // Add renderables
    scene->SetGrid(new PGrid(GRID_OBJ, 5, 1.0f, new Material(glm::vec3(0.2f)), 2, true, glm::vec3(0.0f)));
    scene->SetViewAxisHandle(new PHandle(CAMERA_AXIS_HANDLE, 45.0f, 6, false, glm::vec3(50, 50, 0)));

    scene->AddMesh(new VPlane(BG_PLANE_OBJ, 20.0f, defaultMat));
    scene->AddMesh(new VPlane("brickwall", 2.0f, scene->GetMaterial("brickwall"), glm::vec3(5, 2, 0), glm::vec3(90, 0, 0)));

    if (defaultCubes)
    {
        //scene->Addentity(CAMERA_AXIS_HANDLE, new Cube(15.0f, scene->GetShader(LINE_SHADER), glm::vec3(1.0f), 6, true, glm::vec3(100, 100, 0)));
        scene->AddMesh(new VCube("cube1", 1.0f, new Material(glm::vec3(0, 1, 0)), glm::vec3(-5, 0, -5), glm::vec3(0, 45, 0), glm::vec3(1, 2, 1)));
        scene->AddMesh(new VCube("cube2", 1.0f, new Material(glm::vec3(1, 0, 0)), glm::vec3(-5, 0, -3), glm::vec3(45, 0, 0), glm::vec3(2, 1, 1)));
        scene->AddMesh(new VCube("cube3", 1.0f, new Material(glm::vec3(0, 0, 1)), glm::vec3(-5, 0, -1), glm::vec3(0, 0, 45), glm::vec3(1, 1, 2)));
        scene->AddMesh(new VCube("cube4", 1.0f, new Material(glm::vec3(0, 1, 1)), glm::vec3(-5, 0, 1), glm::vec3(0, 45, 45), glm::vec3(1, 2, 2)));
        scene->AddMesh(new VCube("cube5", 1.0f, new Material(glm::vec3(1, 0, 1)), glm::vec3(-5, 0, 3), glm::vec3(45, 0, 45), glm::vec3(2, 1, 2)));
        scene->AddMesh(new VCube("cube6", 1.0f, new Material(glm::vec3(1, 1, 0)), glm::vec3(-5, 0, 5), glm::vec3(45, 45, 0), glm::vec3(2, 2, 1)));
    }
}

// Handles calculating the number of frames per second in state
// ----------------------------------------------------------------
void CalculateFPS(State* state, double& lastSecond, int& numFrames)
{
    // Calculate FPS
    double currentTime = glfwGetTime();
    numFrames++;

    // Check whether a full second has passed since the last update
    if (currentTime - lastSecond >= 1.0)
    {
        // Calculate FPS and Frame Length in ms
        state->fps = std::to_string(numFrames);
        std::string frameTime = std::to_string(1000.0f / numFrames);
        state->frameTime = frameTime.substr(0, frameTime.find(".") + 3) + "ms";

        // Reset frame times
        numFrames = 0;
        lastSecond = currentTime;
    }
}

// Handles quitting from the window
bool HandleWindowQuit(Window* window, State* state)
{
    if (!glfwWindowShouldClose(window->GetWindow()))
        return false;

    // If state is saved, break
    if (state->IsSaved())
        return true;

    // If state is not saved, set a popup
    pfd::button res = pfd::message(
        "Unsaved Changes",
        "You have made unsaved changes to the scene. Would you like to save them before quitting?",
        pfd::choice::yes_no_cancel,
        pfd::icon::warning
    ).result();

    switch (res)
    {
    case pfd::button::yes:
        // If the user requested to save first, run the save command
        window->GetInput()->RunCommand("SAVE_SCENE");
        return true;
        break;
    case pfd::button::no:
        // If the user did not request to save, don't run the save command
        return true;
        break;
    case pfd::button::cancel:
    default:
        // If the user canceled, do not quit
        glfwSetWindowShouldClose(window->GetWindow(), GLFW_FALSE);
        return false;
    }
}