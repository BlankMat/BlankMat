#include "loadShader.h"

// Reads the shader at the given location to the given out string
// Help reading file line by line from:
// https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
// --------------------------------------------------------------
std::string ReadShaderFromFile(std::string fileName)
{
    std::string shader = "";

    // Open the given file
    std::ifstream file(SHADER_DIR + fileName);
    if (file.is_open())
    {
        // Read each individual line of the file
        std::string line = "";
        while (std::getline(file, line))
        {
            shader.append(line);
            shader.append("\n");
        }
        shader.append("\0");
        file.close();
    }
    // If the given file could not be found, print an error message
    else
    {
        std::cout << "Error: Attempted to read shader at location [" << fileName << "], but no such file exists. Shader loading failed." << std::endl;
    }

    return shader;
}

// Loads shaders for the program from files and returns the shader program ID
// --------------------------------------------------------------------------
unsigned int LoadShaders(std::string vertexFile, std::string geometryFile, std::string fragmentFile)
{
    // vertex shader
    // -------------
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Load vertex shader from source.vs
    std::string tempVertShader = ReadShaderFromFile(vertexFile);
    const char* vertexShaderFile = tempVertShader.c_str();

    glShaderSource(vertexShader, 1, &vertexShaderFile, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    // --------------------
    // End of vertex shader

    // geometry shader
    // ---------------
    unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    // Load fragment shader from source.fs
    std::string tempGeomShader = ReadShaderFromFile(geometryFile);
    const char* geometryShaderFile = tempGeomShader.c_str();

    glShaderSource(geometryShader, 1, &geometryShaderFile, NULL);
    glCompileShader(geometryShader);

    // check for shader compile errors
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    // ----------------------
    // End of geometry shader

    // fragment shader
    // ---------------
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load fragment shader from source.fs
    std::string tempFragShader = ReadShaderFromFile(fragmentFile);
    const char* fragmentShaderFile = tempFragShader.c_str();

    glShaderSource(fragmentShader, 1, &fragmentShaderFile, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    // ----------------------
    // End of fragment shader

    // link shaders
    // ------------
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
    // ----------------------
    // End of linking shaders

    return shaderProgram;
}

unsigned int LoadShaderProgram(bool isPhong)
{
    // Load phong shader
    if (isPhong)
        return LoadShaders("phongShader.vert", "phongShader.geom", "phongShader.frag");
    // Load gouraud shader
    else
        return LoadShaders("gouraudShader.vert", "gouraudShader.geom", "gouraudShader.frag");
}