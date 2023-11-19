#pragma once
#include "glIncludes.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#define IS_BOOL std::is_same<T, bool>::value
#define IS_INT std::is_same<T, int>::value
#define IS_FLOAT std::is_same<T, float>::value
#define IS_VEC3 std::is_same<T, glm::vec3>::value
#define IS_STRING std::is_same<T, std::string>::value

/// <summary>
/// Returns the euler degrees of the direction vector with respect to the up vector
/// http://geom3d.com/data/documents/Calculation=20of=20Euler=20angles.pdf
/// </summary>
/// <param name="dir">The direction (x) to rotate towards</param>
/// <param name="up">The up axis is by default y, but can be changed</param>
/// <returns></returns>
static glm::vec3 GetRotationDegrees(const glm::vec3& dir, const glm::vec3& up = glm::vec3(0,1,0))
{
    //return glm::vec3(atan2(dir.x, dir.z), asin(dir.y), 0.0f);
    /*
    According to "Calculation of Euler Angles" by Prokopi Nikolaev,
        variables defined here use y as the up (Z1 in the formula)
        dir = x -> X1
        up = y -> Z1
        right = z -> Y1
    */

    /*
    glm::vec3 right = glm::normalize(glm::cross(dir, up));
    float y1xz = glm::sqrt(up.x * up.x + up.z * up.z);
    if (y1xz > 0.0f)
    {
        return glm::vec3(
            glm::degrees(atan2f(right.x * up.z - right.z * up.x, dir.x * up.z - dir.z * up.x)),
            glm::degrees(atan2f(y1xz, up.y)),
            glm::degrees(-atan2f(-up.x, up.z))
        );
    }
    else
    {
        return glm::vec3(
            glm::degrees(0.0f), 
            glm::degrees((up.y > 0.0f ? 0.0f : PI)), 
            glm::degrees(-atan2f(dir.z, dir.x))
        );
    }
    */
    
    glm::vec3 right = glm::normalize(glm::cross(dir, up));
    float rotX = asinf(-dir.y);
    if (cosf(rotX) > 0.0001)
        return glm::degrees(glm::vec3(rotX, atan2f(-dir.x, -dir.z), atan2f(right.y, up.y)));
    return glm::degrees(glm::vec3(rotX, 0.0f, atan2f(-up.x, right.x)));
}

// Taken from https://gamedev.stackexchange.com/questions/115032/how-should-i-rotate-vertices-around-the-origin-on-the-cpu
static const glm::vec4 RotateAround(const glm::vec4& aPointToRotate, const glm::vec4& aRotationCenter, const glm::mat4& aRotationMatrix)
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(aRotationCenter.x, aRotationCenter.y, aRotationCenter.z));
	glm::mat4 invTranslate = glm::inverse(translate);

	// The idea:
	// 1) Translate the object to the center
	// 2) Make the rotation
	// 3) Translate the object back to its original location
	glm::mat4 transform = translate * aRotationMatrix * invTranslate;

	return transform * aPointToRotate;
}

// Returns whether the given file exists
static const bool FileExists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

// Rounds the value to the given precision
static const std::string RoundToPrecision(float _val, size_t _precision)
{
    size_t nzero = _precision + 1;
    std::string str = std::to_string(_val);
    auto new_str = std::string(nzero - std::min(nzero, str.length()), '0') + str;
    return new_str;
}

// Help parsing a string by token from:
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
static const void ParseStringByDelim(std::vector<std::string>& out, const std::string& str, const std::string& delim)
{
    // Make sure out vec is cleared
    out.clear();

    // Parse each part of the string
    size_t spos = 0;
    std::string token;
    std::string tempStr = str;
    while ((spos = tempStr.find(delim)) != std::string::npos) {
        // Parse token
        token = tempStr.substr(0, spos);
        out.push_back(token);
        tempStr.erase(0, spos + delim.length());
    }
    out.push_back(tempStr);
}

// Reads a vector 3 from the given strings, skipping ahead the given offset. Strings must have size >= 4 + offset to be read, and the first element + offset are ignored
static const glm::vec3 ReadVec3FromStrings(const std::vector<std::string>& strings, int offset)
{
    // Avoid errors
    if ((int)strings.size() >= 4 + offset)
        return glm::vec3(std::stof(strings[1 + offset]), std::stof(strings[2 + offset]), std::stof(strings[3 + offset]));
    else
        return glm::vec3();
}

static const bool IsCCW(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	return ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) > 0;
}

/// <summary>
/// Clamps the given value to be within min and max
/// </summary>
/// <param name="value">Value to clamp</param>
/// <param name="min">Minimum</param>
/// <param name="max">Maximum value</param>
/// <returns>Value in range [min, max]</returns>
static const int Clamp(int value, int min, int max)
{
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}

/// <summary>
/// Converts the given decimal value to hexadecimal.
/// Borrowed from: 
/// https://www.geeksforgeeks.org/convert-the-given-rgb-color-code-to-hex-color-code/
/// </summary>
/// <param name="value">Decimal value</param>
/// <returns>Hexadecimal equivalent</returns>
static const std::string DecimalToHex(int value)
{
    // char array to store hexadecimal number
    char hexValue[2];

    // counter for hexadecimal number array
    int i = 0;
    while (value != 0) {

        // temporary variable to store remainder
        int temp = 0;

        // storing remainder in temp variable.
        temp = value % 16;

        // check if temp < 10
        if (temp < 10) {
            hexValue[i] = temp + 48;
            i++;
        }
        else {
            hexValue[i] = temp + 55;
            i++;
        }

        value = value / 16;
    }

    std::string hexCode = "";
    if (i == 2) {
        hexCode.push_back(hexValue[0]);
        hexCode.push_back(hexValue[1]);
    }
    else if (i == 1) {
        hexCode = "0";
        hexCode.push_back(hexValue[0]);
    }
    else if (i == 0)
        hexCode = "00";

    // Return the equivalent
    // hexadecimal color code
    return hexCode;
}

/// <summary>
/// Converts the given vec3 to its hexadecimal equivalent
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <returns>Hex value</returns>
static const std::string Vec3ToHex(const glm::vec3& vec)
{
    int r = Clamp((int)round(vec.r * 255), 0, 255);
    int g = Clamp((int)round(vec.g * 255), 0, 255);
    int b = Clamp((int)round(vec.b * 255), 0, 255);

    std::string hex = "#";
    hex += DecimalToHex(r);
    hex += DecimalToHex(g);
    hex += DecimalToHex(b);
    return hex;
}

static const std::string Vec3ToString(const glm::vec3& vec)
{
    std::ostringstream ss;
    ss << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
    return ss.str();
}

static const glm::vec3 Vec3FromFloats(const float vec[3])
{
    return glm::vec3(vec[0], vec[1], vec[2]);
}

static const glm::vec4 Vec4FromFloats(const float vec[4])
{
    return glm::vec4(vec[0], vec[1], vec[2], vec[3]);
}

static const glm::mat3 Mat3FromFloats(const float vec1[3], const float vec2[3], const float vec3[3])
{
    return glm::mat3(Vec3FromFloats(vec1), Vec3FromFloats(vec2), Vec3FromFloats(vec3));
}

static const glm::mat4 Mat4FromFloats(const float vec1[4], const float vec2[4], const float vec3[4], const float vec4[4])
{
    return glm::mat4(Vec4FromFloats(vec1), Vec4FromFloats(vec2), Vec4FromFloats(vec3), Vec4FromFloats(vec4));
}

static float* FloatsFromVec3(const glm::vec3& vec3)
{
    float vec[3] = { vec3.x, vec3.y, vec3.z };
    return vec;
}

static const glm::vec3 Vec3FromAssimp(const aiColor3D& vec)
{
    return glm::vec3(vec.r, vec.g, vec.b);
}

static const glm::vec3 Vec3FromAssimp(const aiVector3D& vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

static const glm::vec2 Vec2FromAssimp(const aiVector2D& vec)
{
	return glm::vec2(vec.x, vec.y);
}

static const glm::vec2 Vec2FromAssimp(const aiVector3D& vec)
{
	return glm::vec2(vec.x, vec.y);
}

template<typename T>
typename static std::enable_if<IS_STRING, const std::string>::type
TypeToString(T value)
{
    return value;
}

template<typename T>
typename static std::enable_if<IS_INT || IS_FLOAT, const std::string>::type
TypeToString(T value)
{
    return std::to_string(value);
}

template<typename T>
typename static std::enable_if<IS_VEC3, const std::string>::type
TypeToString(T value)
{
    return Vec3ToString(value);
}

template<typename T>
typename static std::enable_if<IS_BOOL, const std::string>::type
TypeToString(T value)
{
    return (value) ? "True" : "False";
}

template<typename T>
typename static std::enable_if<!IS_BOOL && !IS_INT && !IS_FLOAT && !IS_VEC3 && !IS_STRING, const std::string>::type
TypeToString(T value)
{
    return "invalid_type";
}