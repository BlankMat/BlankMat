#pragma once
#include "glIncludes.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <unordered_map>

class Config
{
private:
	std::string mName;
	std::unordered_map<std::string, Config*> mChildConfigs;
	std::unordered_map<std::string, std::string> mConfigStrings;
	std::unordered_map<std::string, glm::vec3> mConfigVecs;
	std::unordered_map<std::string, float> mConfigFloats;
	std::unordered_map<std::string, int> mConfigInts;
	std::unordered_map<std::string, bool> mConfigBools;

	// Returns whether the requested name is a nested config
	const bool IsNested(const std::string& name);

	// Returns the config with the given name
	Config* GetNameTarget(std::string* name);

	// Returns the given config
	Config* GetThisConfig(const std::string& name);
	// Returns the given string
	const std::string GetThisString(const std::string& name);
	// Returns the given vec3
	const glm::vec3 GetThisVec(const std::string& name);
	// Returns the given float
	const float GetThisFloat(const std::string& name);
	// Returns the given int
	const int GetThisInt(const std::string& name);
	// Returns the given bool
	const bool GetThisBool(const std::string& name);

	// Sets the given config to the given value
	void SetThisConfig(const std::string& name, Config* val);
	// Sets the given string to the given value
	void SetThisString(const std::string& name, const std::string& val);
	// Sets the given vec3 to the given value
	void SetThisVec(const std::string& name, const glm::vec3& val);
	// Sets the given float to the given value
	void SetThisFloat(const std::string& name, const float val);
	// Sets the given int to the given value
	void SetThisInt(const std::string& name, const int val);
	// Sets the given bool to the given value
	void SetThisBool(const std::string& name, const bool val);
public:
	// Creates a config with the given name
	Config(const std::string& name) { mName = name; }

	// Returns the config's name
	const std::string GetName() { return mName; }

	// Returns the child configs
	const std::unordered_map<std::string, Config*>& GetConfigs() { return mChildConfigs; }

	// Returns the given config
	Config* GetConfig(const std::string& name);
	// Returns the given string
	const std::string GetString(const std::string& name);
	// Returns the given vec3
	const glm::vec3 GetVec(const std::string& name);
	// Returns the given float
	const float GetFloat(const std::string& name);
	// Returns the given int
	const int GetInt(const std::string& name);
	// Returns the given bool
	const bool GetBool(const std::string& name);

	// Sets the given config to the given value
	void SetConfig(const std::string& name, Config* val);
	// Sets the given string to the given value
	void SetString(const std::string& name, const std::string& val);
	// Sets the given vec3 to the given value
	void SetVec(const std::string& name, const glm::vec3& val);
	// Sets the given float to the given value
	void SetFloat(const std::string& name, const float val);
	// Sets the given int to the given value
	void SetInt(const std::string& name, const int val);
	// Sets the given bool to the given value
	void SetBool(const std::string& name, const bool val);

	// Prints the config
	void Print(const int depth = 0);
	// Resets the config
	void Reset();
};