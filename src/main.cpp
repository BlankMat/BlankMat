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
    ModelReader::LoadModel(scene, FileSystem::GetPath(MODELS_DIR) + config->GetString("model.file"), 
        config->GetVec("model.pos"), config->GetVec("model.rot"), config->GetVec("model.scale"));

    state->GetSel()->SetTransformHandle(scene->GetTransformHandle());

    // Load shaders
    LoadShaders(scene, config->GetConfig("shaders"));
    state->defaultMat = scene->GetDefaultMaterial();

    // Load default scene
    LoadDefaultScene(scene, state, state->defaultMat, config->GetBool("defaultCubes"), config->GetConfig("camera"), config->GetConfig("light"));

    // Add GUIs
    LoadGUIs(window, state, scene, window->GetInput(), config);

    // Main program loop should run until the program is exited and the changes are saved or ignored
    while (true)
    {
        // Poll events at the start of the loop
        glfwPollEvents();

        // If the window should close, handle exiting
        if (HandleWindowQuit(window, state))
            break;

        // Calculate FPS and deltaTime
        state->GetTimer()->HandleFrame();

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

    // Deallocate all GLFW and ImGui resources.
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

    /*
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
    */

    // Reset viewport
    window->ResetViewport();

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
// ----------------------------------------------------------------------------------------------------------------------------------
void LoadDefaultScene(Scene* scene, State* state, Material* defaultMat, bool defaultCubes, Config* cameraConfig, Config* lightConfig)
{
    MaterialContainer* mats = scene->GetMaterials();
    TextureContainer* textures = scene->GetTextures();

    // Add light and camera
    scene->SetCamera(state->GetActionStack(), cameraConfig);
    scene->SetLight(new PLightCube("globalLight", 1.0f, mats, textures, lightConfig));

    scene->AddMesh(new VPlane(BG_PLANE_OBJ, 20.0f, defaultMat));
    scene->AddMesh(new VPlane("brickwall", 2.0f, scene->GetMaterial("brickwall"), glm::vec3(5, 2, 0), glm::vec3(90, 0, 0)));

    if (defaultCubes)
    {
        // Add cube materials
        Material* greenMat = mats->AddMaterial(new Material("green", glm::vec3(0, 1, 0), textures, false));
        Material* redMat = mats->AddMaterial(new Material("red", glm::vec3(1, 0, 0), textures, false));
        Material* blueMat = mats->AddMaterial(new Material("blue", glm::vec3(0, 0, 1), textures, false));
        Material* cyanMat = mats->AddMaterial(new Material("cyan", glm::vec3(0, 1, 1), textures, false));
        Material* purpleMat = mats->AddMaterial(new Material("purple", glm::vec3(1, 0, 1), textures, false));
        Material* yellowMat = mats->AddMaterial(new Material("yellow", glm::vec3(1, 1, 0), textures, false));

        scene->AddMesh(new VCube("cube1", 1.0f, greenMat, glm::vec3(-5, 0, -5), glm::vec3(0, 45, 0), glm::vec3(1, 2, 1)));
        scene->AddMesh(new VCube("cube2", 1.0f, redMat, glm::vec3(-5, 0, -3), glm::vec3(45, 0, 0), glm::vec3(2, 1, 1)));
        scene->AddMesh(new VCube("cube3", 1.0f, blueMat, glm::vec3(-5, 0, -1), glm::vec3(0, 0, 45), glm::vec3(1, 1, 2)));
        scene->AddMesh(new VCube("cube4", 1.0f, cyanMat, glm::vec3(-5, 0, 1), glm::vec3(0, 45, 45), glm::vec3(1, 2, 2)));
        scene->AddMesh(new VCube("cube5", 1.0f, purpleMat, glm::vec3(-5, 0, 3), glm::vec3(45, 0, 45), glm::vec3(2, 1, 2)));
        scene->AddMesh(new VCube("cube6", 1.0f, yellowMat, glm::vec3(-5, 0, 5), glm::vec3(45, 45, 0), glm::vec3(2, 2, 1)));
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