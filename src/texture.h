#pragma once
#include <string>

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;

	Texture(unsigned int _id = -1, std::string _type = "", std::string _path = "");
};