#include "config.h"

// ------------------------------------------ PRIVATE -----------------------------------------
// Returns whether the requested name is a nested config
bool Config::IsNested(std::string name)
{
	return (name.find('.') != std::string::npos);
}

Config* Config::GetNameTarget(std::string* name)
{
	// If the name is not nested, return this
	if (!IsNested(*name))
		return this;

	// Break the name into its two parts
	size_t index = name->find_first_of('.');
	std::string config = name->substr(0, index);
	*name = name->substr(index+1);

	// Check whether the child requested exists
	Config* child = GetThisConfig(config);
	return (child != nullptr) ? child : this;
}

// Returns the given config
Config* Config::GetThisConfig(std::string name)
{
	if (mChildConfigs.find(name) != mChildConfigs.end())
		return mChildConfigs[name];
	return nullptr;
}

// Returns the given string
std::string Config::GetThisString(std::string name)
{
	if (mConfigStrings.find(name) != mConfigStrings.end())
		return mConfigStrings[name];
	return "";
}

// Returns the given vec3
glm::vec3 Config::GetThisVec(std::string name)
{
	if (mConfigVecs.find(name) != mConfigVecs.end())
		return mConfigVecs[name];
	return glm::vec3(0.0f);
}

// Returns the given float
float Config::GetThisFloat(std::string name)
{
	if (mConfigFloats.find(name) != mConfigFloats.end())
		return mConfigFloats[name];
	return -1.0f;
}

// Returns the given int
int Config::GetThisInt(std::string name)
{
	if (mConfigInts.find(name) != mConfigInts.end())
		return mConfigInts[name];
	return -1;
}

// Returns the given bool
bool Config::GetThisBool(std::string name)
{
	if (mConfigBools.find(name) != mConfigBools.end())
		return mConfigBools[name];
	return false;
}

// Sets the given config to the given value
void Config::SetThisConfig(std::string name, Config* val)
{
	if (mChildConfigs.find(name) == mChildConfigs.end())
		mChildConfigs.emplace(name, val);
	else
		mChildConfigs[name] = val;
}

// Sets the given string to the given value
void Config::SetThisString(std::string name, std::string val)
{
	if (mConfigStrings.find(name) == mConfigStrings.end())
		mConfigStrings.emplace(name, val);
	else
		mConfigStrings[name] = val;
}

// Sets the given vec3 to the given value
void Config::SetThisVec(std::string name, glm::vec3 val)
{
	if (mConfigVecs.find(name) == mConfigVecs.end())
		mConfigVecs.emplace(name, val);
	else
		mConfigVecs[name] = val;
}

// Sets the given float to the given value
void Config::SetThisFloat(std::string name, float val)
{
	if (mConfigFloats.find(name) == mConfigFloats.end())
		mConfigFloats.emplace(name, val);
	else
		mConfigFloats[name] = val;
}

// Sets the given int to the given value
void Config::SetThisInt(std::string name, int val)
{
	if (mConfigInts.find(name) == mConfigInts.end())
		mConfigInts.emplace(name, val);
	else
		mConfigInts[name] = val;
}

// Sets the given bool to the given value
void Config::SetThisBool(std::string name, bool val)
{
	if (mConfigBools.find(name) == mConfigBools.end())
		mConfigBools.emplace(name, val);
	else
		mConfigBools[name] = val;
}

// ------------------------------------------ PUBLIC ------------------------------------------
// Returns the given config
Config* Config::GetConfig(std::string name)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->GetConfig(name);
	return config->GetThisConfig(name);
}

// Returns the given string
std::string Config::GetString(std::string name)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->GetString(name);
	return config->GetThisString(name);
}

// Returns the given vec3
glm::vec3 Config::GetVec(std::string name)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->GetVec(name);
	return config->GetThisVec(name);
}

// Returns the given float
float Config::GetFloat(std::string name)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->GetFloat(name);
	return config->GetThisFloat(name);
}

// Returns the given int
int Config::GetInt(std::string name)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->GetInt(name);
	return config->GetThisInt(name);
}

// Returns the given bool
bool Config::GetBool(std::string name)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->GetBool(name);
	return config->GetThisBool(name);
}

// Sets the given config to the given value
void Config::SetConfig(std::string name, Config* val)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->SetConfig(name, val);
	return config->SetThisConfig(name, val);
}

// Sets the given string to the given value
void Config::SetString(std::string name, std::string val)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->SetString(name, val);
	return config->SetThisString(name, val);
}

// Sets the given vec3 to the given value
void Config::SetVec(std::string name, glm::vec3 val)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->SetVec(name, val);
	return config->SetThisVec(name, val);
}

// Sets the given float to the given value
void Config::SetFloat(std::string name, float val)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->SetFloat(name, val);
	return config->SetThisFloat(name, val);
}

// Sets the given int to the given value
void Config::SetInt(std::string name, int val)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->SetInt(name, val);
	return config->SetThisInt(name, val);
}

// Sets the given bool to the given value
void Config::SetBool(std::string name, bool val)
{
	Config* config = GetNameTarget(&name);
	if (config != this)
		return config->SetBool(name, val);
	return config->SetThisBool(name, val);
}

// Prints the config
void Config::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		std::cout << "-";
	std::cout << "Members:" << std::endl;
	for (auto iter = mConfigBools.begin(); iter != mConfigBools.end(); ++iter)
	{
		for (int i = 0; i < depth; i++)
			std::cout << "-";
		std::cout << "Bool [" << iter->first << "]: " << iter->second << std::endl;
	}
	for (auto iter = mConfigInts.begin(); iter != mConfigInts.end(); ++iter)
	{
		for (int i = 0; i < depth; i++)
			std::cout << "-";
		std::cout << "Int [" << iter->first << "]: " << iter->second << std::endl;
	}
	for (auto iter = mConfigStrings.begin(); iter != mConfigStrings.end(); ++iter)
	{
		for (int i = 0; i < depth; i++)
			std::cout << "-";
		std::cout << "String [" << iter->first << "]: " << iter->second << std::endl;
	}
	for (auto iter = mConfigFloats.begin(); iter != mConfigFloats.end(); ++iter)
	{
		for (int i = 0; i < depth; i++)
			std::cout << "-";
		std::cout << "Float [" << iter->first << "]: " << iter->second << std::endl;
	}
	for (auto iter = mConfigVecs.begin(); iter != mConfigVecs.end(); ++iter)
	{
		for (int i = 0; i < depth; i++)
			std::cout << "-";
		std::cout << "Vec3 [" << iter->first << "]: " << Vec3ToString(iter->second) << std::endl;
	}

	for (int i = 0; i < depth; i++)
		std::cout << "-";
	std::cout << "Children:" << std::endl;
	for (auto iter = mChildConfigs.begin(); iter != mChildConfigs.end(); ++iter)
	{
		for (int i = 0; i < depth; i++)
			std::cout << "-";
		std::cout << "Child [" << iter->first << "]: ";
		iter->second->Print(depth + 1);
	}
}

// Resets the config
void Config::Reset()
{
	mConfigStrings.clear();
	mConfigVecs.clear();
	mConfigFloats.clear();
	mConfigInts.clear();
	mConfigBools.clear();
	for (auto iter = mChildConfigs.begin(); iter != mChildConfigs.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			iter->second->Reset();
			delete iter->second;
		}
	}
	mChildConfigs.clear();
}