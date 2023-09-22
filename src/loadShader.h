#pragma once
#include "glIncludes.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

std::string ReadShaderFromFile(std::string filename);
unsigned int LoadShaders(std::string vertexFile, std::string geometryFile, std::string fragmentFile);
unsigned int LoadShaderProgram(bool isPhong);