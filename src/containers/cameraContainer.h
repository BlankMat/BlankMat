#pragma once
#include "rendering/camera.h"
#include "interfaces/iContainer.h"
#include "interaction/actionStack.h"

class CameraContainer : public IContainer<Camera>
{
protected:
	/// <summary>
	/// Action stack used to connect camera to UI
	/// </summary>
	ActionStack* mActionStack = nullptr;

	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	void RenameItem(Camera* item, const std::string& name) override
	{
		item->SetName(name);
	}

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Camera*> ReadItem(std::ifstream& file) override
	{
		// Set up variables to store read information
		std::string name = "main";
		float fov = 45.0f;
		float nearClip = 0.1f;
		float farClip = 100.0f;
		glm::vec3 pos = glm::vec3(5.0f, 5.0f, 5.0f);
		glm::vec3 dir = glm::vec3(-5.0f, -5.0f, -5.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 bgColor = glm::vec3(0.3f, 0.4f, 0.4f);
		float orthSize = 10.0f;
		bool isPerspective = true;

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
			if (parse[0] == "CAMERA")
				name = parse[1];
			else if (parse[0] == "fov")
				fov = std::stof(parse[1]);
			else if (parse[0] == "nearclip")
				nearClip = std::stof(parse[1]);
			else if (parse[0] == "farclip")
				farClip = std::stof(parse[1]);
			else if (parse[0] == "pos")
				pos = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "dir")
				dir = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "up")
				up = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "bgcolor")
				bgColor = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "orthsize")
				orthSize = std::stof(parse[1]);
			else if (parse[0] == "perspective")
				isPerspective = (parse[1] == "1");
		}

		return std::pair<std::string, Camera*>(name, new Camera(mActionStack, UnscopeName(name), UnscopeScope(name), fov, nearClip, farClip, pos, dir, up, bgColor, orthSize, isPerspective));
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Camera* item, std::ofstream& file) override
	{
		file << "CAMERA " << key << std::endl;
		file << "fov " << item->GetFOV() << std::endl;
		file << "nearclip " << item->GetNearClip() << std::endl;
		file << "farclip " << item->GetFarClip() << std::endl;
		file << "pos " << Vec3ToString(item->GetPos()) << std::endl;
		file << "dir " << Vec3ToString(item->GetDir()) << std::endl;
		file << "up " << Vec3ToString(item->GetUp()) << std::endl;
		file << "bgcolor " << Vec3ToString(item->GetBGColor()) << std::endl;
		file << "orthsize " << item->GetOrthSize() << std::endl;
		file << "perspective " << (int)item->IsPerspective() << std::endl;
	}
public:
	/// <summary>
	/// Constructs a camera container with the action stack used to track UI variables
	/// </summary>
	/// <param name="actionStack">ActionStack of the app</param>
	explicit CameraContainer(ActionStack* actionStack)
		: mActionStack(actionStack)
	{}
};