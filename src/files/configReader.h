#pragma once
#include "fileSystem.h"
#include "mathLib.h"
#include "config.h"
#include <json/json.h>
#include <string>
#include <iostream>
#include <fstream>

class ConfigReader
{
private:
	static glm::vec3 JsonValueToVec3(const Json::Value& val)
	{
		if (val.size() != 3 || !val.isArray())
			return glm::vec3(0.0f);
		return glm::vec3(val[0].asFloat(), val[1].asFloat(), val[2].asFloat());
	}

	static void AddValueToConfig(const Json::Value& val, const std::string key, Config* config)
	{
		// Get vec3
		if (val.size() == 3 && val.isArray())
			config->SetVec(key, JsonValueToVec3(val));
		else if (val.isDouble())
			config->SetFloat(key, val.asFloat());
		else if (val.isInt())
			config->SetInt(key, val.asInt());
		else if (val.isString())
			config->SetString(key, val.asString().c_str());
		else if (val.isBool())
			config->SetBool(key, val.asBool());
	}

	// Recursively reads the given JSON tree, keeping track of the parent key (if any)
	static Config* ReadJSONTree(const Json::Value& root, const std::string key, Config* config)
	{
		// If the node doesn't have children, read the value to the config passed in
		if (root.size() <= 0 || (root.isArray() && root.size() == 3 && key != ""))
		{
			AddValueToConfig(root, key, config);
		}
		// If this node has children, read their values
		else
		{
			Config* tempConfig = new Config(key);
			// If this is the root, set config as parent config
			if (config == nullptr)
				config = tempConfig;
			// Or add this config as a subconfig of the parent
			else
				config->SetConfig(key, tempConfig);

			for (auto iter = root.begin(); iter != root.end(); ++iter)
			{
				ReadJSONTree(*iter, iter.key().asString().c_str(), tempConfig);
			}
		}
		return config;
	}
public:
	static Config* ReadFile(std::string fileName)
	{
		// Don't read a file that doesn't exist
		if (!FileExists(fileName))
		{
			std::cout << "ERROR::CONFIGREADER::DNE File " << fileName << " does not exist or cannot be accessed." << std::endl;
			return nullptr;
		}

		std::ifstream f(fileName);
		Json::Value root;
		f >> root;

		// If the root cannot be read
		if (root.isNull() || root.size() == 0)
		{
			std::cout << "ERROR::CONFIGREADER::NULL File " << fileName << " is not valid JSON or was read incorrectly." << std::endl;
			return nullptr;
		}

		std::cout << "Read JSON file " << fileName <<  ". Contents:" << std::endl;
		std::cout << root.toStyledString() << std::endl;
		return ReadJSONTree(root, "root", nullptr);
	}
};