#include "main.h"

int main()
{
    // Init window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, APP_NAME);
    if (window == nullptr)
        return -1;

    // Read Options
    // ------------
    std::cout << "Starting program, reading options from " << FileSystem::GetPath(CONFIG_FILE) << std::endl;
    Options options = ReadOptions(FileSystem::GetPath(CONFIG_FILE));

    // Create scene
    // ------------
    ModelScene* scene = new ModelScene();
    scene->CreateShader(DEFAULT_SHADER, options.shader, options.shaderGeom);
    scene->CreateShader(LIGHT_CUBE_SHADER, false);
    scene->CreateShader(LINE_SHADER, false);
    scene->SetCamera(&options);
    scene->SetLight(new LightCube(1.0f, scene->GetShader(LIGHT_CUBE_SHADER), &options));

    scene->AddEntity(BG_PLANE_OBJ, new Plane(20.0f, true, scene->GetShader(DEFAULT_SHADER), options.defaultMat.kd, false), true);
    scene->AddEntity(GRID_OBJ, new Grid(5, 1.0f, scene->GetShader(LINE_SHADER), glm::vec3(0.2f), 2, true, glm::vec3(0.0f)), true);
    scene->AddEntity(TRANSFORM_HANDLE, new TransformHandle(0.5f, scene->GetShader(LINE_SHADER), 6, true, glm::vec3(0.0f)));
    scene->AddEntity(CAMERA_AXIS_HANDLE, new TransformHandle(45.0f, scene->GetShader(LINE_SHADER), 6, false, glm::vec3(50, 50, 0)));
    //scene->Addentity(CAMERA_AXIS_HANDLE, new Cube(15.0f, scene->GetShader(LINE_SHADER), glm::vec3(1.0f), 6, true, glm::vec3(100, 100, 0)));
    scene->AddEntity("cube1", new Cube(1.0f, scene->GetShader(LINE_SHADER), glm::vec3(0,1,0), 0.0f, false, glm::vec3(-5, 0, -5), glm::vec3(0, 45, 0), glm::vec3(1, 2, 1)));
    scene->AddEntity("cube2", new Cube(1.0f, scene->GetShader(LINE_SHADER), glm::vec3(1,0,0), 0.0f, false, glm::vec3(-5, 0, -3), glm::vec3(45, 0, 0), glm::vec3(2, 1, 1)));
    scene->AddEntity("cube3", new Cube(1.0f, scene->GetShader(LINE_SHADER), glm::vec3(0,0,1), 0.0f, false, glm::vec3(-5, 0, -1), glm::vec3(0, 0, 45), glm::vec3(1, 1, 2)));
    scene->AddEntity("cube4", new Cube(1.0f, scene->GetShader(LINE_SHADER), glm::vec3(0,1,1), 0.0f, false, glm::vec3(-5, 0, 1), glm::vec3(0, 45, 45), glm::vec3(1, 2, 2)));
    scene->AddEntity("cube5", new Cube(1.0f, scene->GetShader(LINE_SHADER), glm::vec3(1,0,1), 0.0f, false, glm::vec3(-5, 0, 3), glm::vec3(45, 0, 45), glm::vec3(2, 1, 2)));
    scene->AddEntity("cube6", new Cube(1.0f, scene->GetShader(LINE_SHADER), glm::vec3(1,1,0), 0.0f, false, glm::vec3(-5, 0, 5), glm::vec3(45, 45, 0), glm::vec3(2, 2, 1)));

    // Read mesh
    // ---------
    Model* curModel = new Model(FileSystem::GetPath(MODELS_DIR + options.objName));
    scene->SetModel(curModel);
    curModel->SetMeshShaders(scene->GetShader(DEFAULT_SHADER));
    curModel->SetPos(options.objPos);

    // Enable wireframe if requested in options
    OpenGLEnableWireframe(options.wireframe);

    // Init variables to track user input. Speed constants declared in order:
    // CamMove, CamTurn, ModelMove, ModelTurn, ModelScale, MouseMove, MouseTurn
    SpeedConsts speeds = SpeedConsts(2.0f, 1.0f, 0.3f, 30.0f, 1.0f, 0.1f, 0.1f);
    InputLocks locks = InputLocks();
    int prevX = -1;
    int prevY = -1;

    // Get selection
    Selection sel = Selection();
    sel.SetSelMode(SelMode::MESH);
    sel.SetTool(Tool::SELECT);

    // Track time
    double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = 0.0f;

    float gammaKeyPressed = false;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Get deltaTime
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);

        // Process input and render
        ProcessInput(window, scene, &sel, &locks, &options, &speeds, deltaTime, &prevX, &prevY);
        if (glfwGetKey(window->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
        {
            options.gamma = !options.gamma;
            gammaKeyPressed = true;
        }
        if (glfwGetKey(window->GetWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE)
        {
            gammaKeyPressed = false;
        }
        // Process changes in selections
        if (sel.newSelVerts.size() != 0 || sel.removedSelVerts.size() != 0) {
            locks.reselect = false;
        }

        // Update VAO on rerender call
        if (locks.rerender) {
            // Reset rerender
            locks.rerender = false;
        }

        // Render
        OpenGLDraw(window, scene, &sel, &options);

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
    delete window;
    return 0;
}

// Draws the current scene
// -----------------------
void OpenGLDraw(Window* window, IScene* scene, Selection* sel, Options* options)
{
    glm::vec3 bgColor = scene->GetCamera()->GetBGColor();
    glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the object
    Shader* curShader = scene->GetShader(DEFAULT_SHADER);
    scene->UseShader(DEFAULT_SHADER);

    // Send window scale
    window->CalcWindowSize();
    curShader->SetVec2("WIN_SCALE", glm::vec2(window->GetWidth(), window->GetHeight()));

    // Apply lighting
    glm::vec3 lightOffset = scene->GetLight()->GetOffset();
    scene->GetLight()->SetPos(options->isRotatingLight
        ? glm::vec3(lightOffset.x * sin(glfwGetTime()), lightOffset.y, lightOffset.z * cos(glfwGetTime()))
        : lightOffset);
    glm::vec3 lightColor = scene->GetLight()->GetBaseColor();
    scene->GetLight()->SetColor(options->isDiscoLight
        ? glm::vec3(lightColor.x * sin(glfwGetTime()-PI*0.5f), lightColor.y * sin(glfwGetTime()), lightColor.z * sin(glfwGetTime()+PI*0.5f))
        : lightColor);

    // Set lighting uniforms
    scene->GetLight()->UpdateShader(curShader);
    curShader->SetBool("gamma", options->gamma);
    curShader->SetVec3("viewPos", scene->GetCamera()->GetPos());

    // Draw the scene
    scene->Draw(window);
}
