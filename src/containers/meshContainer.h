#pragma once
#include "rendering/mesh.h"
#include "interfaces/iContainer.h"
#include "containers/materialContainer.h"

class MeshContainer : public IContainer<Mesh>
{
protected:
	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	void RenameItem(Mesh* item, const std::string& name) override
	{
		item->SetName(name);
	}

	/// <summary>
	/// Reads the vertices for the current mesh in the input file
	/// </summary>
	/// <param name="outVerts">Indices list to add to</param>
	/// <param name="file">File to read from</param>
	static void ReadVertices(std::vector<Vertex>& outVerts, std::ifstream& file)
	{
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

			// Don't parse incomplete lines
			if (parse.size() > 11)
			{
				// Parse lines
				glm::vec3 pos = ReadVec3FromStrings(parse, 1);
				glm::vec3 normal = ReadVec3FromStrings(parse, 4);
				glm::vec3 tangent = ReadVec3FromStrings(parse, 7);
				glm::vec2 texCoords = ReadVec2FromStrings(parse, 10);
				outVerts.push_back(Vertex(pos, normal, texCoords, tangent));
			}
		}
	}

	/// <summary>
	/// Reads the indices for the current mesh in the input file
	/// </summary>
	/// <param name="outInds">Indices list to add to</param>
	/// <param name="file">File to read from</param>
	static void ReadIndices(std::vector<unsigned int>& outInds, std::ifstream& file)
	{
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
			for (unsigned int i = 0; i < parse.size(); i++)
				outInds.push_back(std::stoi(parse[i]));
		}
	}

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Mesh*> ReadItem(std::ifstream& file) override
	{
		// Set up variables to store read information
		std::string name = "default";
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 rot = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		std::string material = "default";
		bool enabled = true;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

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
			if (parse[0] == "MESH" && parse.size() > 1)
				name = parse[1];
			else if (parse[0] == "pos" && parse.size() > 3)
				pos = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "rot" && parse.size() > 3)
				rot = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "scale" && parse.size() > 3)
				scale = ReadVec3FromStrings(parse, 1);
			else if (parse[0] == "material" && parse.size() > 1)
				material = parse[1];
			else if (parse[0] == "enabled" && parse.size() > 1)
				enabled = (parse[1] == "1");
			else if (parse[0] == "vertices")
				ReadVertices(vertices, file);
			else if (parse[0] == "indices")
			{
				ReadIndices(indices, file);
				break;
			}
		}

		// Combine information into a new mesh
		Mesh* tempMesh = new Mesh(UnscopeName(name), UnscopeScope(name), vertices, indices, material);
		tempMesh->SetPos(pos);
		tempMesh->SetRot(rot);
		tempMesh->SetScale(scale);
		tempMesh->Enable(enabled);
		return std::pair<std::string, Mesh*>(name, tempMesh);
	}

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	void WriteItem(const std::string& key, Mesh* item, std::ofstream& file) override
	{
		file << "MESH " << key << std::endl;
		file << "pos " << Vec3ToString(item->GetPos()) << std::endl;
		file << "rot " << Vec3ToString(item->GetRot()) << std::endl;
		file << "scale " << Vec3ToString(item->GetScale()) << std::endl;
		if (item->GetMaterial() != nullptr)
			file << "material " << item->GetMaterial()->GetScopedName() << std::endl;
		file << "enabled " << (int)item->IsEnabled() << std::endl;

		// Write all vertices
		const auto& verts = item->GetVertices();
		file << "vertices " << verts.size() << std::endl;
		for (unsigned int i = 0; i < verts.size(); i++)
		{
			Vertex vert = verts[i];
			file << i << " " << Vec3ToString(verts[i].pos) << " ";
			file << Vec3ToString(verts[i].normal) << " ";
			file << Vec3ToString(verts[i].tangent) << " ";
			file << Vec2ToString(verts[i].texCoords) << std::endl;
		}
		file << std::endl;

		// Write all indices
		const auto& inds = item->GetIndices();
		file << "indices " << inds.size() << std::endl;
		unsigned int i = 0;
		for (unsigned int i = 0; i < inds.size(); i++)
		{
			// Group together indices by 6s (2 tris)
			if (i != 0 && i % 6 == 0)
				file << std::endl;
			file << inds[i] << " ";
		}
		file << std::endl;
	}

public:
	/// <summary>
	/// Loads the materials of meshes in this container
	/// </summary>
	/// <param name="materials">Material list of the scene</param>
	void LoadMaterials(MaterialContainer* materials)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			iter->second->LoadMaterial(materials);
		}
	}

	/// <summary>
	/// Draws the contents of the container
	/// </summary>
	/// <param name="shader">Shader to draw with</param>
	/// <param name="state">Global state</param>
	/// <param name="defaultMat">Default material</param>
	/// <param name="viewProj">View Projection matrix</param>
	/// <param name="drawMats">Whether to draw materials</param>
	void Draw(Shader* shader, State* state, Material* defaultMat, const glm::mat4& viewProj, bool drawMats = false)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			iter->second->Draw(shader, state, defaultMat, viewProj, drawMats);
		}
	}
};