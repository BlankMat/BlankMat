#include "openGLHelper.h"

// Sets the OpenGL Uniform Location IDs
// ------------------------------------
void ProgramIDs::GetUniformIDs()
{
    matrixID = glGetUniformLocation(shaderProgram, "MVP");
    modelID = glGetUniformLocation(shaderProgram, "Model");
    normalModelID = glGetUniformLocation(shaderProgram, "NormalModel");
    lightPosID = glGetUniformLocation(shaderProgram, "LightPos");
    lightColorID = glGetUniformLocation(shaderProgram, "LightColor");
    viewPosID = glGetUniformLocation(shaderProgram, "ViewPos");
    ambientStrengthID = glGetUniformLocation(shaderProgram, "AmbientStrength");
    specularStrengthID = glGetUniformLocation(shaderProgram, "SpecularStrength");
    winScaleID = glGetUniformLocation(shaderProgram, "WIN_SCALE");
}

// Opens a OpenGL window with the given name
// -----------------------------------------
GLFWwindow* OpenGLInitWindow(int width, int height, std::string name)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const char* windowName = name.c_str();

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    return window;
}

// Initializes the VAO, VBO, and EBO with the given data
// -----------------------------------------------------
void OpenGLInitBuffers(ProgramIDs* ids, int vertsSize, float* vertices, int indicesSize, unsigned int* indices)
{
    glGenBuffers(1, &ids->EBO);
    glGenBuffers(1, &ids->VBO);

    // Bind VAO
    glGenVertexArrays(1, &ids->VAO);
    glBindVertexArray(ids->VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, ids->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertsSize * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(indices[0]), indices, GL_STATIC_DRAW);

    // Selection
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, VERT_SHADER_SIZE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERT_SHADER_SIZE * sizeof(float), (void*)(1 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERT_SHADER_SIZE * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERT_SHADER_SIZE * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    // Get uniform locations
    ids->GetUniformIDs();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Draws the current scene
// -----------------------
void OpenGLDraw(Scene* scene, Selection* sel, ProgramIDs* ids, int indicesSize, unsigned int* indices)
{
    glClearColor(scene->bgColor.r / 255.0f, scene->bgColor.g / 255.0f, scene->bgColor.b / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the object
    glUseProgram(ids->shaderProgram);

    // Apply MVP
    scene->CalcInvMVP();
    glm::mat4 model = scene->GetModelMatrix();
    glm::mat4 view = scene->GetViewMatrix();
    glm::mat4 projection = scene->GetProjectionMatrix();
    glm::mat3 normalModel = glm::mat3(glm::transpose(glm::inverse(model)));
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(ids->matrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(ids->modelID, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix3fv(ids->normalModelID, 1, GL_FALSE, &normalModel[0][0]);

    // Send window scale
    glm::vec2 winScale = glm::vec2(SCR_WIDTH, SCR_HEIGHT);
    glUniform2fv(ids->winScaleID, 1, &winScale[0]);

    // Apply lighting
    glUniform1f(ids->ambientStrengthID, scene->GetLight()->ka);
    glUniform1f(ids->specularStrengthID, scene->GetLight()->ks);
    glUniform3fv(ids->lightPosID, 1, &scene->GetLight()->pos[0]);
    glUniform3fv(ids->lightColorID, 1, &scene->GetLight()->color[0]);
    glUniform3fv(ids->viewPosID, 1, &scene->GetCamera()->pos[0]);
    glBindVertexArray(ids->VAO);

    // Draw indexed EBO
    glDrawElements(GL_TRIANGLES, indicesSize * sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// De-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
void OpenGLCleanup(ProgramIDs* ids)
{
    glDeleteVertexArrays(1, &ids->VAO);
    glDeleteBuffers(1, &ids->VBO);
    glDeleteBuffers(1, &ids->EBO);
    glDeleteProgram(ids->shaderProgram);
}

// Enable or disable wireframe
// ------------------------------------------
void OpenGLEnableWireframe(bool enable)
{
    if (enable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Disable culling
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // Enable culling
        glEnable(GL_CULL_FACE);

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}