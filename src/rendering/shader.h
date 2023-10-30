#pragma once
#include "glIncludes.h"
#include "files/fileSystem.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Shader utility class borrowed from
// CC BY-NC 4.0 license https://creativecommons.org/licenses/by-nc/4.0/
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
class Shader
{
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(GLuint shader, const std::string& type);
public:
    std::string name;
    unsigned int ID;

    // constructor generates the shader on the fly
    Shader(const std::string& path, bool loadGeom = false);
    // constructor generates the shader on the fly
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    // Cleans up the shader's info
    ~Shader();
    // activate the shader
    void Use();

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w);
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
};
