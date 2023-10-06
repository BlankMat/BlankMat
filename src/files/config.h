#pragma once
#include "glIncludes.h"
#include "mathLib.h"
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
	bool IsNested(std::string name);
	Config* GetNameTarget(std::string* name);

	// Returns the given config
	Config* GetThisConfig(std::string name);
	// Returns the given string
	std::string GetThisString(std::string name);
	// Returns the given vec3
	glm::vec3 GetThisVec(std::string name);
	// Returns the given float
	float GetThisFloat(std::string name);
	// Returns the given int
	int GetThisInt(std::string name);
	// Returns the given bool
	bool GetThisBool(std::string name);

	// Sets the given config to the given value
	void SetThisConfig(std::string name, Config* val);
	// Sets the given string to the given value
	void SetThisString(std::string name, std::string val);
	// Sets the given vec3 to the given value
	void SetThisVec(std::string name, glm::vec3 val);
	// Sets the given float to the given value
	void SetThisFloat(std::string name, float val);
	// Sets the given int to the given value
	void SetThisInt(std::string name, int val);
	// Sets the given bool to the given value
	void SetThisBool(std::string name, bool val);
public:
	// Creates a config with the given name
	Config(std::string name) { mName = name; }

	// Returns the config's name
	std::string GetName() { return mName; }

	// Returns the given config
	Config* GetConfig(std::string name);
	// Returns the given string
	std::string GetString(std::string name);
	// Returns the given vec3
	glm::vec3 GetVec(std::string name);
	// Returns the given float
	float GetFloat(std::string name);
	// Returns the given int
	int GetInt(std::string name);
	// Returns the given bool
	bool GetBool(std::string name);

	// Sets the given config to the given value
	void SetConfig(std::string name, Config* val);
	// Sets the given string to the given value
	void SetString(std::string name, std::string val);
	// Sets the given vec3 to the given value
	void SetVec(std::string name, glm::vec3 val);
	// Sets the given float to the given value
	void SetFloat(std::string name, float val);
	// Sets the given int to the given value
	void SetInt(std::string name, int val);
	// Sets the given bool to the given value
	void SetBool(std::string name, bool val);

	// Prints the config
	void Print(int depth = 0);
	// Resets the config
	void Reset();
};