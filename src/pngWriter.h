#pragma once
#include "glIncludes.h"
#include "lodepng.h"
#include <string>
#include <vector>

struct Png {
	std::vector<glm::vec4> img;
	unsigned int width;
	unsigned int height;

	void WritePngToFile(std::string location, std::string fileName);

	Png(unsigned int _width, unsigned int _height);
};