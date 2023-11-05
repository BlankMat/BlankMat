#pragma once
#include "fileSystem.h"
#include "utils.h"
#include "config.h"
#include <json/json.h>
#include <string>
#include <iostream>
#include <fstream>

/// <summary>
/// Static class for reading JSON files into Config classes
/// </summary>
class ConfigReader
{
private:
	/// <summary>
	/// Converts a JSON value to a glm vec3
	/// </summary>
	/// <param name="val">Value to convert</param>
	/// <returns>vec3 of Value</returns>
	static glm::vec3 JsonValueToVec3(const Json::Value& val)
	{
		if (val.size() != 3 || !val.isArray())
			return glm::vec3(0.0f);
		return glm::vec3(val[0].asFloat(), val[1].asFloat(), val[2].asFloat());
	}

	/// <summary>
	/// Adds the given key-value pair to the config
	/// </summary>
	/// <param name="val">JSON value</param>
	/// <param name="key">Key of the value (name of the property)</param>
	/// <param name="config">Config to add the value to</param>
	static void AddValueToConfig(const Json::Value& val, const std::string& key, Config* config)
	{
		// Get vec3
		if (val.size() == 3 && val.type() == Json::ValueType::arrayValue)
			config->SetVec(key, JsonValueToVec3(val));
		else if (val.type() == Json::ValueType::intValue || val.type() == Json::ValueType::uintValue)
			config->SetInt(key, val.asInt());
		else if (val.type() == Json::ValueType::realValue)
			config->SetFloat(key, val.asFloat());
		else if (val.type() == Json::ValueType::stringValue)
			config->SetString(key, val.asString().c_str());
		else if (val.type() == Json::ValueType::booleanValue)
			config->SetBool(key, val.asBool());
	}

	/// <summary>
	/// Recursively reads the given JSON tree, keeping track of the parent key (if any)
	/// </summary>
	/// <param name="root">Root JSON value to start reading from</param>
	/// <param name="key">Name of the parent json value</param>
	/// <param name="config">Parent config (or nullptr if this is the root)</param>
	/// <returns>Complete Config read from this level</returns>
	static Config* ReadJSONTree(const Json::Value& root, const std::string& key, Config* config)
	{
		// If the node doesn't have children, read the value to the config passed in
		if (root.size() <= 0 || (root.isArray() && root.size() == 3 && key != ""))
		{
			AddValueToConfig(root, key, config);
		}
		// If this node has children, read their values
		else
		{
			auto* tempConfig = new Config(key);
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
	/// <summary>
	/// Reads the given file into a Config class
	/// </summary>
	/// <param name="fileName">Filename to open and read</param>
	/// <returns>Complete Config class</returns>
	static Config* ReadFile(const std::string& fileName)
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
		if (root.isNull() || root.empty())
		{
			std::cout << "ERROR::CONFIGREADER::NULL File " << fileName << " is not valid JSON or was read incorrectly." << std::endl;
			return nullptr;
		}

		std::cout << "Read file " << fileName << std::endl;
		return ReadJSONTree(root, "root", nullptr);
	}
};