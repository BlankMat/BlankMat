#include "main.h"

int main()
{
    // Read Configs
    std::cout << "Starting program, reading options from " << FileSystem::GetPath(CONFIG_JSON) << std::endl;
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

    // Load shaders
    LoadShaders(scene, shadersConfig);
    Material* defaultMat = scene->SetDefaultMaterial("default");

    // Load default scene
    LoadDefaultScene(scene, defaultMat, config->GetBool("defaultCubes"), config->GetConfig("camera"), config->GetConfig("light"));

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

    // Add GUIs
    window->AddGUI(new GUIDebugToolsWindow(state, scene, true));
    window->AddGUI(new GUIHierarchyWindow(state, scene, true));
    window->AddGUI(new GUIInspectorWindow(state, scene, true));
    window->AddGUI(new GUILightViewer(state, scene, true));
    window->AddGUI(new GUIMaterialViewer(state, scene, true));
    window->AddGUI(new GUIMaterialEditor(state, scene, true));
    window->AddGUI(new GUIMenuBarWindow(true));
    window->AddGUI(new GUIToolbarWindow(state, scene, true));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
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

        // Draw GUI
        window->DrawGUI();
        //ImGui::ShowDemoWindow();

        // Process input and render
        ProcessInput(window, scene, state, &locks, deltaTime, &prevX, &prevY);

        // Render
        OpenGLDraw(window, state, scene);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
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

    // Get the shader
    Shader* curShader = scene->GetShader(scene->GetCurShader());
    scene->UseShader();
    
    // Render depth map
    glViewport(0, 0, state->depthMapSize, state->depthMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, state->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    curShader->SetInt("shadowMap", state->depthMapFBO);

    // Reset viewport
    glViewport(0, 0, window->GetWidth(), window->GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply lighting
    glm::vec3 lightOffset = scene->GetLight()->GetOffset();
    scene->GetLight()->SetPos(state->isRotatingLight
        ? glm::vec3(lightOffset.x * sin(glfwGetTime()), lightOffset.y, lightOffset.z * cos(glfwGetTime()))
        : lightOffset);
    glm::vec3 lightColor = scene->GetLight()->GetBaseColor();
    scene->GetLight()->SetColor(state->isDiscoLight
        ? glm::vec3(lightColor.x * sin(glfwGetTime()-PI*0.5f), lightColor.y * sin(glfwGetTime()), lightColor.z * sin(glfwGetTime()+PI*0.5f))
        : lightColor);

    OpenGLEnableWireframe(scene->GetCamera()->IsWireframe());

    // Draw the scene
    scene->Draw(window, curShader);
}

// Opens all defined GUIs
// ------------------------------------------------------
void LoadGUIs(Window* window, State* state, Scene* scene)
{
    window->AddGUI(new GUIDebugToolsWindow(state, scene, true));
    window->AddGUI(new GUIHierarchyWindow(state, scene, true));
    window->AddGUI(new GUIInspectorWindow(state, scene, true));
    window->AddGUI(new GUILightViewer(state, scene, true));
    window->AddGUI(new GUIMaterialViewer(state, scene, true));
    window->AddGUI(new GUIMaterialEditor(state, scene, true));
    window->AddGUI(new GUIMenuBarWindow(true));
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
void LoadDefaultScene(Scene* scene, Material* defaultMat, bool defaultCubes, Config* cameraConfig, Config* lightConfig)
{
    // Add light and camera
    scene->SetCamera(cameraConfig);
    scene->SetLight(new PLightCube("globalLight", 1.0f, lightConfig));

    // Add renderables
    scene->AddEntity(new PPlane(BG_PLANE_OBJ, 20.0f, true, defaultMat, false), true);
    scene->AddEntity(new PGrid(GRID_OBJ, 5, 1.0f, new Material(glm::vec3(0.2f)), 2, true, glm::vec3(0.0f)), true);
    scene->AddEntity(new PHandle(TRANSFORM_HANDLE, 0.5f, 6, true, glm::vec3(0.0f)));
    scene->AddEntity(new PHandle(CAMERA_AXIS_HANDLE, 45.0f, 6, false, glm::vec3(50, 50, 0)));
    scene->AddMesh(new VPlane("brickwall", 2.0f, scene->GetMaterial("brickwall"), glm::vec3(5, 2, 0), glm::vec3(90, 0, 0)));
    //scene->AddMesh(new VPlane("bgPlane", 10.0f, defaultShader, defaultMat, defaultMat, state));

    if (defaultCubes)
    {
        //scene->Addentity(CAMERA_AXIS_HANDLE, new Cube(15.0f, scene->GetShader(LINE_SHADER), glm::vec3(1.0f), 6, true, glm::vec3(100, 100, 0)));
        scene->AddEntity(new PCube("cube1", 1.0f, new Material(glm::vec3(0, 1, 0)), 0.0f, false, glm::vec3(-5, 0, -5), glm::vec3(0, 45, 0), glm::vec3(1, 2, 1)));
        scene->AddEntity(new PCube("cube2", 1.0f, new Material(glm::vec3(1, 0, 0)), 0.0f, false, glm::vec3(-5, 0, -3), glm::vec3(45, 0, 0), glm::vec3(2, 1, 1)));
        scene->AddEntity(new PCube("cube3", 1.0f, new Material(glm::vec3(0, 0, 1)), 0.0f, false, glm::vec3(-5, 0, -1), glm::vec3(0, 0, 45), glm::vec3(1, 1, 2)));
        scene->AddEntity(new PCube("cube4", 1.0f, new Material(glm::vec3(0, 1, 1)), 0.0f, false, glm::vec3(-5, 0, 1), glm::vec3(0, 45, 45), glm::vec3(1, 2, 2)));
        scene->AddEntity(new PCube("cube5", 1.0f, new Material(glm::vec3(1, 0, 1)), 0.0f, false, glm::vec3(-5, 0, 3), glm::vec3(45, 0, 45), glm::vec3(2, 1, 2)));
        scene->AddEntity(new PCube("cube6", 1.0f, new Material(glm::vec3(1, 1, 0)), 0.0f, false, glm::vec3(-5, 0, 5), glm::vec3(45, 45, 0), glm::vec3(2, 2, 1)));
    }
}