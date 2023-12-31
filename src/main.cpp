#include "main.h"

int main(int argc, char* argv[])
{
    // Read and print input params
    std::cout << "Opened BlankMat with " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; i++)
        std::cout << "[" << i << "]: " << argv[i] << std::endl;

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

    // Load shaders
    LoadShaders(scene, config->GetConfig("shaders"));
    state->defaultMat = scene->GetDefaultMaterial();
    scene->SetLightingMode(LightingMode::TEXTURED);
    state->GetSel()->SetTransformHandle(scene->GetTransformHandle());

    // If an argument was passed for a starting file, attempt to load it
    if (argc > 1)
    {
        SceneReader::ReadScene(scene, argv[1], true);
    }
    // If opened without arguments, load demo scene
    else
    {
        SceneReader::ReadScene(scene, FileSystem::GetPath(MODELS_DIR) + config->GetString("model.file"), true);
        scene->LoadMaterials(config->GetConfig("materials"));

        // Load default scene
        LoadDefaultScene(scene, state, scene->GetDefaultMaterial(), config->GetBool("defaultCubes"), config->GetConfig("camera"), config->GetConfig("light"));
    }

    // Main program loop should run until the program is exited and the changes are saved or ignored
    while (true)
    {
        // Poll events at the start of the loop
        glfwPollEvents();

        // If the window should close, handle exiting
        if (HandleWindowQuit(window, state))
            break;

        // Only handle rendering if requested
        if (state->shouldRender)
        {
            // Calculate FPS and deltaTime
            state->GetTimer()->HandleFrame();

            // Render
            OpenGLDraw(window, state, scene);

            // Draw GUI
            window->DrawGUI(state);
        }

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

// Loads all defined shaders
// --------------------------------------------------
void LoadShaders(Scene* scene, Config* shaderConfig)
{
    double startTime = Timer::Start();
    std::unordered_map<std::string, Config*> shaders = shaderConfig->GetConfigs();
    for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
    {
        scene->CreateShader(iter->first, iter->second);
    }
    Timer::Time(startTime, "Shaders loaded");
}

// Loads the default scene
// ----------------------------------------------------------------------------------------------------------------------------------
void LoadDefaultScene(Scene* scene, State* state, Material* defaultMat, bool defaultCubes, Config* cameraConfig, Config* lightConfig)
{
    MaterialContainer* mats = scene->GetMaterials();
    TextureContainer* textures = scene->GetTextures();

    // Add light and camera
    scene->AddCamera("main", new Camera(state->GetActionStack(), cameraConfig), true);
    scene->AddLight("global", new PLightCube(1.0f, mats, textures, lightConfig), true);

    scene->AddMesh(new VPlane(BG_PLANE_OBJ, "", 20.0f, defaultMat));
    scene->AddMesh(new VPlane("brickwall", "", 2.0f, scene->GetMaterial("brickwall"), glm::vec3(5, 2, 0), glm::vec3(90, 0, 0)));

    if (defaultCubes)
    {
        // Add cube materials
        Material* greenMat = mats->AddMaterial(new Material("green", "", textures, glm::vec3(0, 1, 0), false));
        Material* redMat = mats->AddMaterial(new Material("red", "", textures, glm::vec3(1, 0, 0), false));
        Material* blueMat = mats->AddMaterial(new Material("blue", "", textures, glm::vec3(0, 0, 1), false));
        Material* cyanMat = mats->AddMaterial(new Material("cyan", "", textures, glm::vec3(0, 1, 1), false));
        Material* purpleMat = mats->AddMaterial(new Material("purple", "", textures, glm::vec3(1, 0, 1), false));
        Material* yellowMat = mats->AddMaterial(new Material("yellow", "", textures, glm::vec3(1, 1, 0), false));

        scene->AddMesh(new VCube("cube1", "", 1.0f, greenMat, glm::vec3(-5, 0, -5), glm::vec3(0, 45, 0), glm::vec3(1, 2, 1)));
        scene->AddMesh(new VCube("cube2", "", 1.0f, redMat, glm::vec3(-5, 0, -3), glm::vec3(45, 0, 0), glm::vec3(2, 1, 1)));
        scene->AddMesh(new VCube("cube3", "", 1.0f, blueMat, glm::vec3(-5, 0, -1), glm::vec3(0, 0, 45), glm::vec3(1, 1, 2)));
        scene->AddMesh(new VCube("cube4", "", 1.0f, cyanMat, glm::vec3(-5, 0, 1), glm::vec3(0, 45, 45), glm::vec3(1, 2, 2)));
        scene->AddMesh(new VCube("cube5", "", 1.0f, purpleMat, glm::vec3(-5, 0, 3), glm::vec3(45, 0, 45), glm::vec3(2, 1, 2)));
        scene->AddMesh(new VCube("cube6", "", 1.0f, yellowMat, glm::vec3(-5, 0, 5), glm::vec3(45, 45, 0), glm::vec3(2, 2, 1)));
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