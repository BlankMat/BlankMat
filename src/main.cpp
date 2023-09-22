#include "main.h"
#include "openGLHelper.h"

int main()
{
    // Init window
    GLFWwindow* window = OpenGLInitWindow(SCR_WIDTH, SCR_HEIGHT, "BlankMat");
    if (window == nullptr)
        return -1;

    // Read Options
    // ------------
    Options options = ReadOptions(CONFIG_FILE);

    // Build and compile shader program
    // ------------------------------------
    ProgramIDs ids = ProgramIDs();
    ids.shaderProgram = LoadShaderProgram(options.phong == 1);

    // Create scene
    // ------------
    Scene* scene = new Scene();
    scene->SetCameraFromOptions(&options);
    scene->SetLight(new Light(
        glm::vec3(5.0f, 5.0f, 5.0f),        // Light pos
        glm::vec3(-1.0f, -1.0f, -1.0f),     // Light dir
        glm::vec3(1.0f, 1.0f, 1.0f),        // Light color
        0.1f,                               // Ambient strength
        7.0f));                             // Specular strength);
    scene->GetMats()->AddMat("default", options.defaultColor);

    // Read mesh
    // ---------
    Mesh* displayMesh = scene->GetMeshes()->GetAll().begin()->second;

    // Read mesh from file
    ReadObjFromFile(displayMesh, scene->GetMats(), MODELS_DIR, options.objName);
    displayMesh->Scale(glm::vec3(options.objScale, options.objScale, options.objScale));
    displayMesh->SetPos(options.objPos);
    displayMesh->CalcPivot();

    // Load up model into vertice and indice structures
    // Get vertices
    int vertsSize = scene->GetVertCount() * 10;
    int indicesSize = scene->GetIndexCount();
    float* vertices = new float[vertsSize];
    unsigned int* indices = new unsigned int[indicesSize];
    scene->CalcRenderTris();
    scene->GetVAO(vertices, vertsSize, indices, indicesSize);
    scene->CalcInvMVP();

    // Print vertices and indices
    if (options.print == 1) {
        PrintArray("Printing vertices:", vertices, vertsSize, 10);
        PrintArray("Printing indices:", indices, indicesSize, 3);
    }

    // Init VAO, VBO, and EBO
    OpenGLInitBuffers(&ids, vertsSize, vertices, indicesSize, indices);

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
    while (!glfwWindowShouldClose(window))
    {
        // Get deltaTime
        lastTime = currentTime;
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);

        // Process input and render
        ProcessInput(window, scene, &sel, &locks, &options, &speeds, deltaTime, &prevX, &prevY);

        // Process changes in selections
        if (sel.newSelVerts.size() != 0 || sel.removedSelVerts.size() != 0) {
            /*
            std::set<int> selVerts;
            sel.GetSelectedVerts(selVerts);
            std::unordered_map<int, Vertex> verts = scene->GetMeshes()->GetAll().begin()->second->GetVerts();
            for (auto viter = verts.begin(); viter != verts.end(); ++viter) {
                vertices[viter->first * VERT_SHADER_SIZE] = (selVerts.find(viter->first) != selVerts.end()) ? 1.0f : 0.0f;
                std::cout << "Checking if " << viter->first << " is selected: " << (selVerts.find(viter->first) != selVerts.end() ? "true" : "false") << "\n";
            }
            */
            //std::cout << "Selecting [" << sel.newSelVerts.size() << "] new verts, removing [" << sel.removedSelVerts.size() << "]\n";

            for (auto iter = sel.newSelVerts.begin(); iter != sel.newSelVerts.end(); ++iter) {
                vertices[*iter * VERT_SHADER_SIZE] = 1.0f;
            }
            for (auto iter = sel.removedSelVerts.begin(); iter != sel.removedSelVerts.end(); ++iter) {
                vertices[*iter * VERT_SHADER_SIZE] = 0.0f;
            }
            sel.newSelVerts.clear();
            sel.removedSelVerts.clear();
            sel.CalcSelPivot();

            glBindBuffer(GL_ARRAY_BUFFER, ids.VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertsSize * sizeof(vertices[0]), vertices);
            locks.reselect = false;

            //PrintArray("Testing vertex data", vertices, vertsSize, 10);
        }

        // Update VAO on rerender call
        if (locks.rerender) {
            // Clear previous data
            delete[] vertices;
            delete[] indices;

            // Set new data
            vertsSize = scene->GetVertCount() * 10;
            indicesSize = scene->GetIndexCount();
            vertices = new float[vertsSize];
            indices = new unsigned int[indicesSize];

            scene->GetVAO(vertices, vertsSize, indices, indicesSize, &sel);
            OpenGLInitBuffers(&ids, vertsSize, vertices, indicesSize, indices);

            // Reset rerender
            locks.rerender = false;
        }

        // Render
        OpenGLDraw(scene, &sel, &ids, indicesSize, indices);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clear up dynamic memory usage
    // -----------------------------
    OpenGLCleanup(&ids);
    delete[] vertices;
    delete[] indices;
    delete scene;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}