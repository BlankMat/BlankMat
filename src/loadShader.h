#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "glIncludes.h"

std::string ReadShaderFromFile(std::string filename);
unsigned int LoadShaders(std::string vertexFile, std::string geometryFile, std::string fragmentFile);
unsigned int LoadShaderProgram(bool isPhong);