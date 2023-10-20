#pragma once
#include "glIncludes.h"
#include <string>
#include <iostream>
//#include "glIncludes.h"
#include "renderingHelpers.h"

struct Texture
{
	unsigned int id;
	std::string name;
	std::string type;
	std::string path;

	Texture::Texture(unsigned int _id = -1, std::string _type = "", std::string _path = "", std::string _name = "")
		: id(_id), type(_type), path(_path), name(_name) {}

	Texture::Texture(std::string _type, std::string _directory, std::string _path, std::string _name)
		: type(_type), path(_path), name(_name)
	{
		int width,height;
		id = TextureFromFile(_directory, _path,width,height);
	}
};