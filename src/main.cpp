#include "main.h"

int main()
{
    // Read Options
    // ------------
    std::cout << "Starting program, reading options from " << FileSystem::GetPath(CONFIG_JSON) << std::endl;
    Config* config = ConfigReader::ReadFile(FileSystem::GetPath(CONFIG_JSON));
    Config* materialsConfig = ConfigReader::ReadFile(FileSystem::GetPath(MATS_JSON));

    // Init window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, APP_NAME, config->GetConfig("style"));
    if (window == nullptr)
        return -1;

    // Create state
    State* state = new State(new Selection(), config);
    state->GetSel()->SetSelMode(SelMode::MESH);
    state->GetSel()->SetTool(Tool::SELECT);

    // Create scene
    // ------------
    Scene* scene = new Scene();
    scene->SetState(state);
    scene->LoadMaterials(materialsConfig);
    scene->LoadModel(FileSystem::GetPath(MODELS_DIR) + config->GetString("model.file"), 
        config->GetVec("model.pos"), config->GetVec("model.rot"), config->GetVec("model.scale"));

    // Load shaders
    Shader* defaultShader = scene->CreateShader(DEFAULT_SHADER, config->GetConfig("shader"));
    Shader* blinnShader = scene->CreateShader(BLINN_SHADER, false);
    Shader* phongShader = scene->CreateShader(PHONG_SHADER, false);
    Shader* lambertShader = scene->CreateShader(LAMBERT_SHADER, false);
    Shader* blinnFlatShader = scene->CreateShader(BLINN_FLAT_SHADER, false);
    Shader* phongFlatShader = scene->CreateShader(PHONG_FLAT_SHADER, false);
    Shader* lambertFlatShader = scene->CreateShader(LAMBERT_FLAT_SHADER, false);
    Shader* lightCubeShader = scene->CreateShader(LIGHT_CUBE_SHADER, false);
    Shader* lineShader = scene->CreateShader(LINE_SHADER, false);
    Material* defaultMat = scene->GetMaterial("default");
    scene->UseShader(DEFAULT_SHADER);

    // Setup scene
    scene->GetRootNode()->SetShader(defaultShader);
    scene->SetMeshShaders(defaultShader, state);
    scene->SetCamera(config->GetConfig("camera"));
    scene->SetLight(new PLightCube("globalLight", 1.0f, lightCubeShader, defaultMat, state, config->GetConfig("light")));

    // Add renderables
    scene->AddEntity(new PPlane(BG_PLANE_OBJ, 20.0f, true, defaultShader, defaultMat, defaultMat, state, false), true);
    scene->AddEntity(new PGrid(GRID_OBJ, 5, 1.0f, lineShader, new Material(glm::vec3(0.2f)), defaultMat, state, 2, true, glm::vec3(0.0f)), true);
    scene->AddEntity(new PHandle(TRANSFORM_HANDLE, 0.5f, lineShader, defaultMat, state, 6, true, glm::vec3(0.0f)));
    scene->AddEntity(new PHandle(CAMERA_AXIS_HANDLE, 45.0f, lineShader, defaultMat, state, 6, false, glm::vec3(50, 50, 0)));
    scene->AddMesh(new VPlane("brickwall", 2.0f, blinnShader, scene->GetMaterial("brickwall"), defaultMat, state, glm::vec3(5, 2, 0), glm::vec3(90, 0, 0)));

    if (config->GetBool("defaultCubes"))
    {
        //scene->Addentity(CAMERA_AXIS_HANDLE, new Cube(15.0f, scene->GetShader(LINE_SHADER), glm::vec3(1.0f), 6, true, glm::vec3(100, 100, 0)));
        scene->AddEntity(new PCube("cube1", 1.0f, scene->GetShader(LINE_SHADER), new Material(glm::vec3(0, 1, 0)), defaultMat, state, 0.0f, false, glm::vec3(-5, 0, -5), glm::vec3(0, 45, 0), glm::vec3(1, 2, 1)));
        scene->AddEntity(new PCube("cube2", 1.0f, scene->GetShader(LINE_SHADER), new Material(glm::vec3(1, 0, 0)), defaultMat, state, 0.0f, false, glm::vec3(-5, 0, -3), glm::vec3(45, 0, 0), glm::vec3(2, 1, 1)));
        scene->AddEntity(new PCube("cube3", 1.0f, scene->GetShader(LINE_SHADER), new Material(glm::vec3(0, 0, 1)), defaultMat, state, 0.0f, false, glm::vec3(-5, 0, -1), glm::vec3(0, 0, 45), glm::vec3(1, 1, 2)));
        scene->AddEntity(new PCube("cube4", 1.0f, scene->GetShader(LINE_SHADER), new Material(glm::vec3(0, 1, 1)), defaultMat, state, 0.0f, false, glm::vec3(-5, 0, 1), glm::vec3(0, 45, 45), glm::vec3(1, 2, 2)));
        scene->AddEntity(new PCube("cube5", 1.0f, scene->GetShader(LINE_SHADER), new Material(glm::vec3(1, 0, 1)), defaultMat, state, 0.0f, false, glm::vec3(-5, 0, 3), glm::vec3(45, 0, 45), glm::vec3(2, 1, 2)));
        scene->AddEntity(new PCube("cube6", 1.0f, scene->GetShader(LINE_SHADER), new Material(glm::vec3(1, 1, 0)), defaultMat, state, 0.0f, false, glm::vec3(-5, 0, 5), glm::vec3(45, 45, 0), glm::vec3(2, 2, 1)));
    }

    // Enable wireframe if requested in options
    OpenGLEnableWireframe(config->GetBool("camera.wireframe"));

    // Init variables to track user input. Speed constants declared in order:
    InputLocks locks = InputLocks();
    int prevX = -1;
    int prevY = -1;

    // Track time
    double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = 0.0f;

    float gammaKeyPressed = false;

    // Add GUIs
    window->AddGUI(new GUIDebugToolsWindow(state, scene, true));
    window->AddGUI(new GUIMaterialViewer(state, scene, true));
    window->AddGUI(new GUILightViewer(state, scene, true));
    window->AddGUI(new GUIHierarchyWindow(state, scene, true));
    window->AddGUI(new GUIInspectorWindow(state, scene, true));
    window->AddGUI(new GUIMenuBarWindow(true));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Get deltaTime
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);

        // Draw GUI
        window->DrawGUI();
        //ImGui::ShowDemoWindow();

        // Process input and render
        ProcessInput(window, scene, state, &locks, deltaTime, &prevX, &prevY);

        // Render
        OpenGLDraw(window, scene, state);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    // Clear up dynamic memory usage
    // -----------------------------
    delete scene;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete window;
    return 0;
}

// Draws the current scene
// -----------------------
void OpenGLDraw(Window* window, IScene* scene, State* state)
{
    glm::vec3 bgColor = scene->GetCamera()->GetBGColor();
    glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the object
    Shader* curShader = scene->GetShader(scene->GetCurShader());
    scene->UseShader();

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
    scene->Draw(window);
}
