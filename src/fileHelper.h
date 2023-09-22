#pragma once
#include "glIncludes.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

bool FileExists(const std::string& name);
std::string RoundToPrecision(float _val, size_t _precision);
glm::vec3 ReadVec3FromStrings(std::vector<std::string>& strings, int offset = 0);
void ParseStringByDelim(std::vector<std::string>& out, std::string str, std::string delim);
void PrintArray(std::string startText, float array[], int n, int elemsPerLine);
void PrintArray(std::string startText, unsigned int[], int n, int elemsPerLine);