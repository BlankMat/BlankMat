#include "main.h"

int main()
{
    // Init window
    Window* window = new Window(SCR_WIDTH, SCR_HEIGHT, "BlankMat");
    if (window == nullptr)
        return -1;

    // Read Options
    // ------------
    std::cout << "Starting program, reading options from " << FileSystem::GetPath(CONFIG_FILE) << std::endl;
    Options options = ReadOptions(FileSystem::GetPath(CONFIG_FILE));

    // Create scene
    // ------------
    Scene* scene = new Scene();
    scene->CreateShader("default", options.shader, options.shaderGeom);
    scene->CreateShader("lightCube", false);
    scene->CreateShader("line", false);
    scene->SetCamera(&options);
    scene->SetLight(new LightCube(1.0f, scene->GetShader("lightCube"), &options));

    scene->AddDrawable(new Grid(5, 1.0f, scene->GetShader("line"), glm::vec3(0.2f), 2, glm::vec3(0.0f)));
    scene->AddDrawable(new TransformHandle(0.5f, scene->GetShader("line"), 6, glm::vec3(0.0f)));
    scene->AddDrawable(new Plane(20.0f, true, scene->GetShader("line"), options.defaultMat.kd));

    // Read mesh
    // ---------
    scene->SetModel(new Model(FileSystem::GetPath(MODELS_DIR + options.objName)));
    scene->GetModel()->SetMeshShaders(scene->GetShader("default"));

    // Enable wireframe if requested in options
    OpenGLEnableWireframe(options.wireframe == 1);

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
void OpenGLDraw(Window* window, Scene* scene, Selection* sel, Options* options)
{
    glm::vec3 bgColor = scene->GetCamera()->GetBGColor();
    glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the object
    Shader* curShader = scene->GetShader("default");
    scene->UseShader("default");

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
    curShader->SetVec3("ViewPos", scene->GetCamera()->GetPos());

    // Draw the scene
    scene->Draw(window);
}
