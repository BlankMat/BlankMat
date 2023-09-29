#pragma once
#include <string>

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;

	Texture::Texture(unsigned int _id = -1, std::string _type = "", std::string _path = "")
		: id(_id), type(_type), path(_path) {}
};