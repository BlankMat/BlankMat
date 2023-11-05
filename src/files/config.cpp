#include "config.h"

// ------------------------------------------ PRIVATE -----------------------------------------
// Returns whether the requested name is a nested config
bool Config::IsNested(const std::string& name) const
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
Config* Config::GetThisConfig(const std::string& name) const
{
	if (mChildConfigs.find(name) != mChildConfigs.end())
		return mChildConfigs.at(name);
	return nullptr;
}

// Returns the given string
std::string Config::GetThisString(const std::string& name) const
{
	if (mConfigStrings.find(name) != mConfigStrings.end())
		return mConfigStrings.at(name);
	return "";
}

// Returns the given vec3
glm::vec3 Config::GetThisVec(const std::string& name) const
{
	if (mConfigVecs.find(name) != mConfigVecs.end())
		return mConfigVecs.at(name);
	return glm::vec3(0.0f);
}

// Returns the given float
float Config::GetThisFloat(const std::string& name) const
{
	if (mConfigFloats.find(name) != mConfigFloats.end())
		return mConfigFloats.at(name);
	return -1.0f;
}

// Returns the given int
int Config::GetThisInt(const std::string& name) const
{
	if (mConfigInts.find(name) != mConfigInts.end())
		return mConfigInts.at(name);
	return -1;
}

// Returns the given bool
bool Config::GetThisBool(const std::string& name) const
{
	if (mConfigBools.find(name) != mConfigBools.end())
		return mConfigBools.at(name);
	return false;
}

// Sets the given config to the given value
void Config::SetThisConfig(const std::string& name, Config* val)
{
	mChildConfigs.try_emplace(name, val);
}

// Sets the given string to the given value
void Config::SetThisString(const std::string& name, const std::string& val)
{
	mConfigStrings.try_emplace(name, val);
}

// Sets the given vec3 to the given value
void Config::SetThisVec(const std::string& name, const glm::vec3& val)
{
	mConfigVecs.try_emplace(name, val);
}

// Sets the given float to the given value
void Config::SetThisFloat(const std::string& name, const float val)
{
	mConfigFloats.try_emplace(name, val);
}

// Sets the given int to the given value
void Config::SetThisInt(const std::string& name, const int val)
{
	mConfigInts.try_emplace(name, val);
}

// Sets the given bool to the given value
void Config::SetThisBool(const std::string& name, const bool val)
{
	mConfigBools.try_emplace(name, val);
}

// ------------------------------------------ PUBLIC ------------------------------------------
// Returns the given config
Config* Config::GetConfig(const std::string& name)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->GetConfig(tempName);
	return config->GetThisConfig(tempName);
}

// Returns the given string
std::string Config::GetString(const std::string& name)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->GetString(tempName);
	return config->GetThisString(tempName);
}

// Returns the given vec3
glm::vec3 Config::GetVec(const std::string& name)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->GetVec(tempName);
	return config->GetThisVec(tempName);
}

// Returns the given float
float Config::GetFloat(const std::string& name)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->GetFloat(tempName);
	return config->GetThisFloat(tempName);
}

// Returns the given int
int Config::GetInt(const std::string& name)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->GetInt(tempName);
	return config->GetThisInt(tempName);
}

// Returns the given bool
bool Config::GetBool(const std::string& name)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->GetBool(tempName);
	return config->GetThisBool(tempName);
}

// Sets the given config to the given value
void Config::SetConfig(const std::string& name, Config* val)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->SetConfig(tempName, val);
	return config->SetThisConfig(tempName, val);
}

// Sets the given string to the given value
void Config::SetString(const std::string& name, const std::string& val)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->SetString(tempName, val);
	return config->SetThisString(tempName, val);
}

// Sets the given vec3 to the given value
void Config::SetVec(const std::string& name, const glm::vec3& val)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->SetVec(tempName, val);
	return config->SetThisVec(tempName, val);
}

// Sets the given float to the given value
void Config::SetFloat(const std::string& name, const float val)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->SetFloat(tempName, val);
	return config->SetThisFloat(tempName, val);
}

// Sets the given int to the given value
void Config::SetInt(const std::string& name, const int val)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->SetInt(tempName, val);
	return config->SetThisInt(tempName, val);
}

// Sets the given bool to the given value
void Config::SetBool(const std::string& name, const bool val)
{
	std::string tempName = name;
	Config* config = GetNameTarget(&tempName);
	if (config != this)
		return config->SetBool(tempName, val);
	return config->SetThisBool(tempName, val);
}

// Prints the config
void Config::Print(const int depth) const
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