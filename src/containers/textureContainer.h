#pragma once
#include "rendering/texture.h"
#include "interfaces/iContainer.h"

class TextureContainer : public IContainer<Texture>
{
protected:
	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	void RenameItem(Texture* item, const std::string& name) override
	{
		item->SetName(name);
	}

	/// <summary>
	/// Returns whether the item should be skipped or not
	/// </summary>
	/// <param name="item">Item to consider</param>
	/// <returns>Whether the item should be skipped</returns>
	bool SkipItem(Texture* item) override
	{
		return item->IsInternal();
	}

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="scope">Scope to read item in</param>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Texture*> ReadItem(const std::string& scope, std::ifstream& file) override
	{
		// Set up variables to store read information
		std::string name = "default";
		std::string dir = "";
		std::string filename = "";
		TextureType type = TextureType::DIFFUSE;

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
			if (parse[0] == "TEXTURE" && parse.size() > 1)
				name = Scope(parse[1], scope);
			else if (parse[0] == "file" && parse.size() > 1)
				filename = parse[1];
			else if (parse[0] == "dir" && parse.size() > 1)
				dir = parse[1];
			else if (parse[0] == "type" && parse.size() > 1)
				type = (TextureType)std::stoi(parse[1]);
		}

		return std::pair<std::string, Texture*>(name, new Texture(UnscopeName(name), UnscopeScope(name), type, dir, filename, false));
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Texture* item, std::ofstream& file) override
	{
		file << "TEXTURE " << item->GetScopedName() << std::endl;
		file << "file " << item->mFile << std::endl;
		file << "dir " << item->mDir << std::endl;
		file << "type " << (int)item->mType << std::endl;
	}

public:
	/// <summary>
	/// Adds the texture to the container
	/// </summary>
	/// <param name="item">The item to store</param>
	/// <param name="replace">Whether to replace the existing item if this is a duplicate</param>
	/// <returns>The element stored in the container</returns>
	Texture* AddTexture(Texture* item, bool replace = false)
	{
		return Add(item->GetScopedName(), item, replace);
	}

	/// <summary>
	/// Returns whether the given item is deletable (ie. not a default element or internal)
	/// </summary>
	/// <param name="item">Item to consider</param>
	/// <returns>Whether the item can be deleted safely</returns>
	virtual bool IsDeleteable(Texture* item)
	{
		return (Count() > 1 && !SkipItem(item));
	}

	/// <summary>
	/// Initializes a default texture container with default textures
	/// </summary>
	explicit TextureContainer()
	{
		mCurSelectedItem = Add("default_diffuse", new Texture("default_diffuse", "", TextureType::DIFFUSE, glm::vec3(1.0f), true), true);
		Add("default_ambient", new Texture("default_ambient", "", TextureType::AMBIENT, glm::vec3(1.0f), true), true);
		Add("default_specular", new Texture("default_specular", "", TextureType::SPECULAR, glm::vec3(1.0f), true), true);
		Add("default_normal", new Texture("default_normal", "", TextureType::NORMAL, glm::vec3(0.5f, 0.5f, 1.0f), true), true);
		Add("default_height", new Texture("default_height", "", TextureType::HEIGHT, glm::vec3(1.0f), true), true);
		Add("default_alpha", new Texture("default_alpha", "", TextureType::ALPHA, glm::vec3(1.0f), true), true);
	}
};