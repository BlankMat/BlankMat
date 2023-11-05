#include "main.h"

int main()
{
    // Read Configs
    Config* config = ConfigReader::ReadFile(FileSystem::GetPath(CONFIG_JSON));
    Config* materialsConfig = ConfigReader::ReadFile(FileSystem::GetPath(MATS_JSON));
    Config* shadersConfig = ConfigReader::ReadFile(FileSystem::GetPath(SHADERS_JSON));

    // Create state
    State* state = new State(new Selection(), config);

    // Init window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, APP_NAME, config, state);
    if (window == nullptr)
        return -1;

    // Create scene
    Scene* scene = new Scene();
    scene->SetState(state);
    scene->LoadMaterials(materialsConfig);
    scene->LoadModel(FileSystem::GetPath(MODELS_DIR) + config->GetString("model.file"), 
        config->GetVec("model.pos"), config->GetVec("model.rot"), config->GetVec("model.scale"));

    state->GetSel()->SetTransformHandle(new PHandle("transformHandle", 0.5f, 6, true, glm::vec3(0.0f)));
    scene->SetTransformHandle(state->GetSel()->GetTransformHandle());

    // Load shaders
    LoadShaders(scene, shadersConfig);
    state->defaultMat = scene->SetDefaultMaterial("default");

    // Load default scene
    LoadDefaultScene(scene, state, state->defaultMat, config->GetBool("defaultCubes"), config->GetConfig("camera"), config->GetConfig("light"));

    // Add GUIs
    LoadGUIs(window, state, scene);

    // Init variables to track user input.
    InputLocks locks = InputLocks();
    int prevX = -1;
    int prevY = -1;

    // Track time
    double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();
    float deltaTime = 0.0f;
    int numFrames = 0;

    float gammaKeyPressed = false;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Poll events at the start of the loop
        glfwPollEvents();

        // Get deltaTime
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);

        // Calculate FPS
        double curFrameTime = glfwGetTime();
        numFrames++;
        if (currentTime - lastFrameTime >= 1.0)
        {
            // Calculate FPS and Frame Length in ms
            state->fps = std::to_string(numFrames);
            std::string frameTime = std::to_string(1000.0f / numFrames);
            state->frameTime = frameTime.substr(0, frameTime.find(".") + 3) + "ms";

            // Reset frame times
            numFrames = 0;
            lastFrameTime = curFrameTime;
        }

        // Rotate light over time
        glm::vec3 lightOffset = scene->GetLight()->GetOffset();
        if (state->isRotatingLight)
            scene->GetLight()->SetPos(glm::vec3(lightOffset.x * sin(glfwGetTime()), lightOffset.y, lightOffset.z * cos(glfwGetTime())));
        else
            scene->GetLight()->SetPos(lightOffset);

        // Change light color over time
        glm::vec3 lightColor = scene->GetLight()->GetBaseColor();
        if (state->isDiscoLight)
            scene->GetLight()->SetColor(glm::vec3(lightColor.x * sin(glfwGetTime() - PI * 0.5f), lightColor.y * sin(glfwGetTime()), lightColor.z * sin(glfwGetTime() + PI * 0.5f)));
        else
            scene->GetLight()->SetColor(lightColor);

        // Draw GUI
        if (state->drawGUI)
            window->DrawGUI();
        //ImGui::ShowDemoWindow();

        // Process input and render
        ProcessInput(window, scene, state, &locks, deltaTime, &prevX, &prevY);

        // Render
        OpenGLDraw(window, state, scene);

        if (state->drawGUI)
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        // Swap buffers at the end of the loop
        glfwSwapBuffers(window->GetWindow());
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
void LoadGUIs(Window* window, State* state, Scene* scene)
{
    window->AddGUI(new GUILightViewer(state, scene, true));
    window->AddGUI(new GUIMaterialViewer(state, scene, true));
    window->AddGUI(new GUIMaterialEditor(state, scene, true));
    window->AddGUI(new GUIDebugToolsWindow(state, scene, true));
    window->AddGUI(new GUIHierarchyWindow(state, scene, true));
    window->AddGUI(new GUIInspectorWindow(state, scene, true));
    window->AddGUI(new GUIMenuBarWindow(true));
    window->AddGUI(new GUIToolbarWindow(state, scene, true));
    window->AddGUI(new GUIToolModeWindow(state, scene, true));
    window->AddGUI(new GUIActionList(state, scene, true));
}

// Loads all defined shaders
// --------------------------------------------------
void LoadShaders(Scene* scene, Config* shaderConfig)
{
    double loadStartTime = glfwGetTime();

    std::unordered_map<std::string, Config*> shaders = shaderConfig->GetConfigs();
    for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
    {
        scene->CreateShader(iter->first, iter->second);
    }
    scene->UseShader("default");

    double loadEndTime = glfwGetTime();
    double loadTime = loadEndTime - loadStartTime;
    std::cout << "Shaders loaded in " << loadTime << " seconds." << std::endl;
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