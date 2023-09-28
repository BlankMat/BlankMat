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
    scene->SetCamera(&options);
    scene->CreateShader("default", options.shader, options.shaderGeom);
    LightCube* light = new LightCube(
        "lightCube",
        glm::vec3(5.0f, 5.0f, 5.0f),        // Light pos
        glm::vec3(-1.0f, -1.0f, -1.0f),     // Light dir
        glm::vec3(1.0f, 1.0f, 1.0f),        // Light color
        0.1f,                               // Ambient strength
        7.0f);                              // Specular strength
    scene->SetLight(light);
    scene->GetCamera()->bgColor = options.bgColor;

    // Read mesh
    // ---------
    Model* curModel = new Model(FileSystem::GetPath(MODELS_DIR + options.objName));

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
        OpenGLDraw(window, scene, &sel);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    // Clear up dynamic memory usage
    // -----------------------------
    delete curModel;
    delete scene;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    delete window;
    return 0;
}

// Draws the current scene
// -----------------------
void OpenGLDraw(Window* window, Scene* scene, Selection* sel)
{
    glm::vec3 bgColor = scene->GetCamera()->bgColor;
    glClearColor(bgColor.r / 255.0f, bgColor.g / 255.0f, bgColor.b / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the object
    Shader* curShader = scene->GetShader("default");
    scene->UseShader("default");

    // Send window scale
    window->CalcWindowSize();
    curShader->SetVec2("WIN_SCALE", glm::vec2(window->GetWidth(), window->GetHeight()));

    // Apply lighting
    glm::vec3 lightOffset = scene->GetLight()->offset;
    scene->GetLight()->pos = glm::vec3(lightOffset.x * sin(glfwGetTime()), lightOffset.y, lightOffset.z * cos(glfwGetTime()));
    curShader->SetFloat("AmbientStrength", scene->GetLight()->ka);
    curShader->SetFloat("SpecularStrength", scene->GetLight()->ks);
    curShader->SetVec3("LightPos", scene->GetLight()->pos);
    curShader->SetVec3("LightColor", scene->GetLight()->color);
    curShader->SetVec3("ViewPos", scene->GetCamera()->pos);

    // Draw the scene
    scene->Draw(window);
}
