#pragma once
#include "glIncludes.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

// Taken from https://gamedev.stackexchange.com/questions/115032/how-should-i-rotate-vertices-around-the-origin-on-the-cpu
static const glm::vec4 RotateAround(glm::vec4 aPointToRotate, glm::vec4 aRotationCenter, glm::mat4x4 aRotationMatrix)
{
	glm::mat4x4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(aRotationCenter.x, aRotationCenter.y, aRotationCenter.z));
	glm::mat4x4 invTranslate = glm::inverse(translate);

	// The idea:
	// 1) Translate the object to the center
	// 2) Make the rotation
	// 3) Translate the object back to its original location
	glm::mat4x4 transform = translate * aRotationMatrix * invTranslate;

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
static const void ParseStringByDelim(std::vector<std::string>& out, std::string str, std::string delim)
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
static const glm::vec3 ReadVec3FromStrings(std::vector<std::string>& strings, int offset)
{
    // Avoid errors
    if ((int)strings.size() >= 4 + offset)
        return glm::vec3(std::stof(strings[1 + offset]), std::stof(strings[2 + offset]), std::stof(strings[3 + offset]));
    else
        return glm::vec3();
}

static const bool IsCCW(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) > 0;
}

static const std::string Vec3ToString(glm::vec3 vec)
{
    std::ostringstream ss;
    ss << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
    return ss.str();
}

static const glm::vec3 Vec3FromAssimp(aiVector3D vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

static const glm::vec2 Vec2FromAssimp(aiVector2D vec)
{
	return glm::vec2(vec.x, vec.y);
}

static const glm::vec2 Vec2FromAssimp(aiVector3D vec)
{
	return glm::vec2(vec.x, vec.y);
}