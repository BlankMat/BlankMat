#pragma once
#include "rendering/light.h"
#include "primitives/pLightCube.h"
#include "interfaces/iContainer.h"
#include "containers/materialContainer.h"
#include "containers/textureContainer.h"

class LightContainer : public IContainer<Light>
{
protected:
	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	void RenameItem(Light* item, const std::string& name) override
	{
		item->SetName(name);
	}

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Light*> ReadItem(std::ifstream& file) override
	{
		// Set up variables to store read information
		std::string name = "global";
		LightType type = LightType::POINT;
		glm::vec3 pos = glm::vec3(5.0f, 5.0f, 5.0f);
		glm::vec3 dir = glm::vec3(-5.0f, -5.0f, -5.0f);
		glm::vec3 color = glm::vec3(1.0f);
		float kd = 1.0f;
		float ka = 0.1f;
		float ks = 1.0f;
		bool gamma = false;
		bool cube = false;
		float range = 30.0f;
		float spotInner = 25.0f;
		float spotOuter = 35.0f;

		// Read the node
		std::string line;
		while (std::getline(file, line))
		{
			// Break when an empty line is encountered
			if (line == "")
				break;

			std::vector<std::string> parse;
			ParseStringByDelim(parse, line, " ");

			// Don't parse empty lines
			if (parse.empty())
				break;

			// Parse lines
			if (parse[0] == "LIGHT")
				name = parse[1];
			if (parse[0] == "type")
				type = (LightType)std::stoi(parse[1]);
			if (parse[0] == "cube")
				cube = (parse[1] == "1");
			else if (parse[0] == "pos")
				pos = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "dir")
				dir = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "color")
				color = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "kd")
				kd = std::stof(parse[1]);
			else if (parse[0] == "ka")
				ka = std::stof(parse[1]);
			else if (parse[0] == "ks")
				ks = std::stof(parse[1]);
			else if (parse[0] == "gamma")
				gamma = (parse[1] == "1");
			else if (parse[0] == "range")
				range = std::stof(parse[1]);
			else if (parse[0] == "spotouter")
				spotOuter = std::stof(parse[1]);
			else if (parse[0] == "spotinner")
				spotInner = std::stof(parse[1]);
		}

		// If the light stored was a lightcube, construct that instead
		if (cube)
			return std::pair<std::string, Light*>(name, new PLightCube(UnscopeName(name), UnscopeScope(name), type, pos, dir, color, kd, ka, ks, gamma, range, spotInner, spotOuter));
		else
			return std::pair<std::string, Light*>(name, new Light(UnscopeName(name), UnscopeScope(name), type, pos, dir, color, kd, ka, ks, gamma, range, spotInner, spotOuter));
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Light* item, std::ofstream& file) override
	{
		file << "LIGHT " << key << std::endl;
		file << "type " << (int)item->GetType() << std::endl;
		file << "cube " << (int)item->IsCube() << std::endl;
		file << "pos " << Vec3ToString(item->GetPos()) << std::endl;
		file << "dir " << Vec3ToString(item->GetDir()) << std::endl;
		file << "color " << Vec3ToString(item->GetColor()) << std::endl;
		file << "kd " << item->GetKD() << std::endl;
		file << "ka " << item->GetKA() << std::endl;
		file << "ks " << item->GetKS() << std::endl;
		file << "gamma " << item->GetGamma() << std::endl;
		file << "range " << item->GetRange() << std::endl;
		file << "spotinner " << item->GetSpotInnerRadius() << std::endl;
		file << "spotouter " << item->GetSpotOuterRadius() << std::endl;
	}
public:
	/// <summary>
	/// Loads all materials of lights, if they have any
	/// </summary>
	/// <param name="materials">Material list</param>
	/// <param name="textures">Texture list</param>
	void LoadMaterials(MaterialContainer* materials, TextureContainer* textures)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
			iter->second->LoadMaterials(materials, textures);
	}

	/// <summary>
	/// Draws all lights that are renderable
	/// </summary>
	/// <param name="shader">Shader to draw with</param>
	/// <param name="state">Global state of the application</param>
	/// <param name="defaultMat">Default material</param>
	/// <param name="viewProj">View projection matrix</param>
	/// <param name="drawMats">Whether to redraw the materials</param>
	void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats = false)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		if (iter->second != nullptr)
			iter->second->Draw(shader, state, defaultMat, viewProj, drawMats);
	}
};