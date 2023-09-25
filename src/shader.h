#pragma once
#include "glIncludes.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define SHADER_DIR ROOT_DIR "resources/shaders/"

// Shader utility class borrowed from
// CC BY-NC 4.0 license https://creativecommons.org/licenses/by-nc/4.0/
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
class Shader
{
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type);
public:
    unsigned int ID;

    // constructor generates the shader on the fly
    Shader(std::string path, bool loadGeom = false);
    // constructor generates the shader on the fly
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    // activate the shader
    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w);
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};
