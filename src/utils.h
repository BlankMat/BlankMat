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

// 

/// <summary>
/// Rotates the given point around the given center.
/// Borrowed from: https://gamedev.stackexchange.com/questions/115032/how-should-i-rotate-vertices-around-the-origin-on-the-cpu
/// </summary>
/// <param name="aPointToRotate">Point to rotate</param>
/// <param name="aRotationCenter">Point to rotate around</param>
/// <param name="aRotationMatrix">Rotation to apply</param>
/// <returns>Transformed position</returns>
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

/// <summary>
/// Returns the directory portion of the given path
/// </summary>
/// <param name="path">Path to split</param>
/// <returns>Directory of the path</returns>
static std::string GetDirectory(const std::string path)
{
    // Don't parse empty paths
    if (path == "")
        return "";

    size_t lastSlash = path.find_last_of("/\\") + 1;
    return path.substr(0, lastSlash);
}

/// <summary>
/// Returns the filename of the given path
/// </summary>
/// <param name="path">Path to split</param>
/// <returns>Filename of the path</returns>
static std::string GetFileName(const std::string path)
{
    // Don't parse empty paths
    if (path == "")
        return "";

    size_t lastSlash = path.find_last_of("/\\") + 1;
    size_t lastPeriod = path.find_last_of('.');
    return path.substr(lastSlash, lastPeriod - lastSlash);
}

/// <summary>
/// Returns the file extension of the given path
/// </summary>
/// <param name="path">Path to split</param>
/// <returns>Extension of the path</returns>
static std::string GetFileExtension(const std::string path)
{
    // Don't parse empty paths
    if (path == "")
        return "";

    size_t lastPeriod = path.find_last_of('.');
    if (lastPeriod < path.length())
        return path.substr(lastPeriod);
    else
        return "";
}

/// <summary>
/// Returns the file format of the given path (extension without a period)
/// </summary>
/// <param name="path">Path to split</param>
/// <returns>File format of the path</returns>
static std::string GetFileFormat(const std::string path)
{
    // Don't parse empty paths
    if (path == "")
        return "";

    size_t lastPeriod = path.find_last_of('.');
    if (lastPeriod < path.length())
        return path.substr(lastPeriod + 1);
    else
        return "";
}

/// <summary>
/// Splits the given path into its directory, filename, and extension
/// </summary>
/// <param name="path">Path to split</param>
/// <param name="dir">Directory</param>
/// <param name="name">Filename</param>
/// <param name="ext">Extension</param>
static void SplitPath(const std::string path, std::string& dir, std::string& name, std::string& ext)
{
    // Don't parse empty paths
    if (path == "")
        return;

    size_t lastSlash = path.find_last_of("/\\") + 1;
    size_t lastPeriod = path.find_last_of('.');

    dir = path.substr(0, lastSlash);
    name = path.substr(lastSlash, lastPeriod - lastSlash);

    if (lastPeriod < path.length())
        ext = path.substr(lastPeriod);
    else
        ext = "";
}

/// <summary>
/// Returns whether the given file exists
/// </summary>
/// <param name="name">Name of the file</param>
/// <returns>Whether the file exists</returns>
static const bool FileExists(const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

/// <summary>
/// Rounds the given value to the given precision
/// </summary>
/// <param name="value">Value to round</param>
/// <param name="precision">Number of digits to keep</param>
/// <returns>A string representation of the float</returns>
static const std::string RoundToPrecision(float value, size_t precision)
{
    size_t nzero = precision + 1;
    std::string str = std::to_string(value);
    return std::string(nzero - std::min(nzero, str.length()), '0') + str;
}

/// <summary>
/// Converts the given integer to a string of the given length, padding zeros in the beginning
/// </summary>
/// <param name="value">Value to convert</param>
/// <param name="length">Minimum length of result</param>
/// <returns>Padded string</returns>
static const std::string IntToString(int value, size_t length)
{
    std::string str = std::to_string(value);
    return std::string(length - std::min(length, str.length()), '0') + str;
}

/// <summary>
/// Trims leading and trailing whitespace from the given text
/// </summary>
/// <param name="text">Text to trim</param>
/// <returns>Trimmed text</returns>
static std::string TrimWhitespace(const std::string& text)
{
    std::string res = text;
    res.erase(0, res.find_first_not_of(" \t\n\r\f\v"));
    res.erase(res.find_last_not_of(" \t\n\r\f\v") + 1);
    return res;
}

/// <summary>
/// Parses the given string into a list of strings by a given delimiter.
/// Borrowed from: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
/// </summary>
/// <param name="out">List of parsed strings</param>
/// <param name="str">String to parse</param>
/// <param name="delim">Delimiter to break up string by</param>
/// <returns>A list of strings broken up from the original string</returns>
static const void ParseStringByDelim(std::vector<std::string>& out, const std::string& str, const std::string& delim)
{
    // Make sure out vec is cleared
    out.clear();

    // Parse each part of the string
    size_t spos = 0;
    std::string token;
    std::string tempStr = TrimWhitespace(str);
    while ((spos = tempStr.find(delim)) != std::string::npos) {
        // Parse token
        token = tempStr.substr(0, spos);
        out.push_back(token);
        tempStr.erase(0, spos + delim.length());
    }
    out.push_back(tempStr);
}

/// <summary>
/// Creates a padded string of length depth that repeats the given pad
/// </summary>
/// <param name="depth">Number of times to repeat the pad</param>
/// <param name="pad">Character to pad</param>
/// <returns>String of size depth * pad.length</returns>
static const std::string GetPadding(unsigned int depth, const std::string& pad = " ")
{
    std::ostringstream ss;
    for (unsigned int i = 0; i < depth; i++)
        ss << pad;
    return ss.str();
}

/// <summary>
/// Returns whether the name is part of a scope or not
/// </summary>
/// <param name="name">Name</param>
/// <returns>Whether the name is part of a scope</returns>
static bool IsScoped(const std::string& name)
{
    return (name.find("::") < name.length());
}

/// <summary>
/// Scopes the name to the given scope (scope::name)
/// </summary>
/// <param name="name">Name to scope</param>
/// <param name="scope">Scope to move to</param>
/// <returns>scope::name</returns>
static std::string Scope(const std::string& name, const std::string& scope)
{
    if (scope != "")
        return scope + "::" + name;
    return name;
}

/// <summary>
/// Removes the scope, if any, from the name (scope::name)
/// </summary>
/// <param name="name">Scoped name</param>
/// <returns>Name without the scope</returns>
static std::string UnscopeName(const std::string& name)
{
    size_t scopeIndex = name.find("::");
    if (scopeIndex >= name.length())
        return name;
    return name.substr(scopeIndex + 2);
}

/// <summary>
/// Removes the scope, if any, from the name (scope::name)
/// </summary>
/// <param name="name">Scoped name</param>
/// <returns>Scope without the name</returns>
static std::string UnscopeScope(const std::string& name)
{
    size_t scopeIndex = name.find("::");
    if (scopeIndex >= name.length())
        return name;
    return name.substr(0, scopeIndex);
}

/// <summary>
/// Increments the name by 1, adding padded numbers to the end
/// </summary>
/// <param name="name">Name to increment</param>
/// <param name="padLen">Length of the padding after the underscore</param>
/// <returns>Incremented name</returns>
static std::string IncrementName(const std::string& name, int padLen = 3)
{
    size_t numIndex = name.find_last_not_of("0123456789");
    std::string endNums = name.substr(numIndex + 1);

    // If the name does not end in numbers, pad it with zeros
    if (endNums.length() <= 0)
        return name + "_" + GetPadding(padLen, "0");

    // If the name ends in numbers, increment the number by one
    return name.substr(0, numIndex + 1) + IntToString(std::stoi(endNums) + 1, endNums.length());
}

/// <summary>
/// Reads a vec2 from the given strings, skipping ahead the given offset. 
/// Strings must have size >= 2 + offset to be read, and the first element + offset are ignored
/// </summary>
/// <param name="strings">List of strings to read</param>
/// <param name="offset">Offset to start reading strings from</param>
/// <returns>vec2 constructed from the given strings</returns>
static const glm::vec2 ReadVec2FromStrings(const std::vector<std::string>& strings, size_t offset)
{
    // Avoid errors
    if (strings.size() >= 2U + offset)
        return glm::vec2(std::stof(strings[offset]), std::stof(strings[1 + offset]));
    else
        return glm::vec2();
}

/// <summary>
/// Reads a vec3 from the given strings, skipping ahead the given offset. 
/// Strings must have size >= 3 + offset to be read, and the first element + offset are ignored
/// </summary>
/// <param name="strings">List of strings to read</param>
/// <param name="offset">Offset to start reading strings from</param>
/// <returns>vec3 constructed from the given strings</returns>
static const glm::vec3 ReadVec3FromStrings(const std::vector<std::string>& strings, size_t offset)
{
    // Avoid errors
    if (strings.size() >= 3U + offset)
        return glm::vec3(std::stof(strings[offset]), std::stof(strings[1 + offset]), std::stof(strings[2 + offset]));
    else
        return glm::vec3();
}

/// <summary>
/// Returns whether the given points are in counter-clockwise winding order
/// </summary>
/// <param name="a">Point a</param>
/// <param name="b">Point b</param>
/// <param name="c">Point c</param>
/// <returns>Whether the points are in counter-clockwise winding order</returns>
static const bool IsCCW(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	return ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) > 0;
}

/// <summary>
/// Clamps the given value to be between min and max
/// </summary>
/// <typeparam name="T">Type of value</typeparam>
/// <param name="value">Value to clamp</param>
/// <param name="min">Minimum value</param>
/// <param name="max">Maximum value</param>
/// <returns>Value in range [min, max]</returns>
template<typename T = int>
static const T Clamp(T value, T min, T max)
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
/// Borrowed from: https://www.geeksforgeeks.org/convert-the-given-rgb-color-code-to-hex-color-code/
/// </summary>
/// <param name="value">Decimal value</param>
/// <returns>Hexadecimal equivalent</returns>
static const std::string DecimalToHex(int value)
{
    // char array to store hexadecimal number
    char hexValue[2] = {};

    // counter for hexadecimal number array
    int i = 0;
    while (value != 0)
    {
        // Store remainder
        int temp = value % 16;
        value = value / 16;

        hexValue[i] = temp + (temp < 10 ? 48 : 55);
        i++;
    }

    std::string hexCode = "";
    if (i == 2)
    {
        hexCode.push_back(hexValue[0]);
        hexCode.push_back(hexValue[1]);
    }
    else if (i == 1)
    {
        hexCode = "0";
        hexCode.push_back(hexValue[0]);
    }
    else if (i == 0)
    {
        hexCode = "00";
    }

    // Return the equivalent hexadecimal color code
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

/// <summary>
/// Converts the given vec3 to its red, green, and blue value equivalent in [0,255]
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <param name="out">Output RGB array</param>
static void Vec3ToRGB(const glm::vec3& vec, unsigned char out[3])
{
    out[0] = Clamp<unsigned char>((unsigned char)round(vec.r * 255), 0, 255);
    out[1] = Clamp<unsigned char>((unsigned char)round(vec.g * 255), 0, 255);
    out[2] = Clamp<unsigned char>((unsigned char)round(vec.b * 255), 0, 255);
}

/// <summary>
/// Converts the given vec2 to a string
/// </summary>
/// <param name="vec">Vec2 to convert</param>
/// <returns>String representation of the value</returns>
static const std::string Vec2ToString(const glm::vec2& vec)
{
    std::ostringstream ss;
    ss << vec.x << " " << vec.y;
    return ss.str();
}

/// <summary>
/// Converts the given vec3 to a string
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <returns>String representation of the value</returns>
static const std::string Vec3ToString(const glm::vec3& vec)
{
    std::ostringstream ss;
    ss << vec.x << " " << vec.y << " " << vec.z;
    return ss.str();
}

/// <summary>
/// Converts the given vec3 to a comma separated string
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <returns>Display string representation of the value</returns>
static const std::string Vec3ToDisplayString(const glm::vec3& vec)
{
    std::ostringstream ss;
    ss << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
    return ss.str();
}

/// <summary>
/// Converts an array of floats to a vec3
/// </summary>
/// <param name="vec">Floats to convert</param>
/// <returns>Vec3 representation of the float array</returns>
static const glm::vec3 Vec3FromFloats(const float vec[3])
{
    return glm::vec3(vec[0], vec[1], vec[2]);
}

/// <summary>
/// Converts an array of floats to a vec4
/// </summary>
/// <param name="vec">Floats to convert</param>
/// <returns>Vec4 representation of the float array</returns>
static const glm::vec4 Vec4FromFloats(const float vec[4])
{
    return glm::vec4(vec[0], vec[1], vec[2], vec[3]);
}

/// <summary>
/// Converts column arrays of floats to a mat3
/// </summary>
/// <param name="vec1">First column of the matrix</param>
/// <param name="vec2">Second column of the matrix</param>
/// <param name="vec3">Third column of the matrix</param>
/// <returns>Mat3 representation of the floats</returns>
static const glm::mat3 Mat3FromFloats(const float vec1[3], const float vec2[3], const float vec3[3])
{
    return glm::mat3(Vec3FromFloats(vec1), Vec3FromFloats(vec2), Vec3FromFloats(vec3));
}

/// <summary>
/// Converts column arrays of floats to a mat4
/// </summary>
/// <param name="vec1">First column of the matrix</param>
/// <param name="vec2">Second column of the matrix</param>
/// <param name="vec3">Third column of the matrix</param>
/// <param name="vec4">Fourth column of the matrix</param>
/// <returns>Mat4 representation of the floats</returns>
static const glm::mat4 Mat4FromFloats(const float vec1[4], const float vec2[4], const float vec3[4], const float vec4[4])
{
    return glm::mat4(Vec4FromFloats(vec1), Vec4FromFloats(vec2), Vec4FromFloats(vec3), Vec4FromFloats(vec4));
}

/// <summary>
/// Converts the given vec3 to an array of floats
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <returns>Float array representation of the value</returns>
static float* FloatsFromVec3(const glm::vec3& vec3)
{
    float vec[3] = { vec3.x, vec3.y, vec3.z };
    return vec;
}

/// <summary>
/// Converts the given assimp color vec3 to a vec3
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <returns>Vec3 representation of the value</returns>
static const glm::vec3 Vec3FromAssimp(const aiColor3D& vec)
{
    return glm::vec3(vec.r, vec.g, vec.b);
}

/// <summary>
/// Converts the given assimp vec3 to a vec3
/// </summary>
/// <param name="vec">Vec3 to convert</param>
/// <returns>Vec3 representation of the value</returns>
static const glm::vec3 Vec3FromAssimp(const aiVector3D& vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

/// <summary>
/// Converts the given assimp vec2 to a vec2
/// </summary>
/// <param name="vec">Vec2 to convert</param>
/// <returns>Vec2 representation of the value</returns>
static const glm::vec2 Vec2FromAssimp(const aiVector2D& vec)
{
	return glm::vec2(vec.x, vec.y);
}

/// <summary>
/// Converts the given assimp vec3 to a vec2, ignoring the b/z channel
/// </summary>
/// <param name="vec">Vec2 to convert</param>
/// <returns>Vec2 representation of the value</returns>
static const glm::vec2 Vec2FromAssimp(const aiVector3D& vec)
{
	return glm::vec2(vec.x, vec.y);
}

/// <summary>
/// Converts the given glm vec2 to Assimp
/// </summary>
/// <param name="vec">Vector 2</param>
/// <returns>Assimp Vector 2</returns>
static const aiVector2D Vec2ToAssimp(const glm::vec2& vec)
{
    return aiVector2D(vec.x, vec.y);
}

/// <summary>
/// Converts the given glm vec3 to Assimp
/// </summary>
/// <param name="vec">Vector 3</param>
/// <returns>Assimp Vector 3</returns>
static const aiVector3D Vec3ToAssimp(const glm::vec3& vec)
{
    return aiVector3D(vec.x, vec.y, vec.z);
}

/// <summary>
/// Converts the given glm vec3 color to Assimp
/// </summary>
/// <param name="vec">Color 3</param>
/// <returns>Assimp Color 3</returns>
static const aiColor3D ColorToAssimp(const glm::vec3& vec)
{
    return aiColor3D(vec.x, vec.y, vec.z);
}

/// <summary>
/// Converts the given glm matrix to Assimp
/// </summary>
/// <param name="mat">Matrix 4</param>
/// <returns>Assimp Matrix 4x4</returns>
static const aiMatrix4x4 Mat4ToAssimp(const glm::mat4& mat)
{
    // a,b,c,d in assimp is the row, 1,2,3,4 is the column
    return aiMatrix4x4(
        mat[0][0], mat[1][0], mat[2][0], mat[3][0], 
        mat[0][1], mat[1][1], mat[2][1], mat[3][1],
        mat[0][2], mat[1][2], mat[2][2], mat[3][2],
        mat[0][3], mat[1][3], mat[2][3], mat[3][3]
    );
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
    return Vec3ToDisplayString(value);
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