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

	Texture::Texture(const unsigned int _id = -1, const std::string& _type = "", const std::string& _path = "", const std::string& _name = "")
		: id(_id), type(_type), path(_path), name(_name) {}

	Texture::Texture(const std::string& _type, const std::string& _directory, const std::string& _path, const std::string& _name)
		: type(_type), path(_path), name(_name)
	{
		int width,height;
		id = TextureFromFile(_directory, _path,width,height);
	}
};