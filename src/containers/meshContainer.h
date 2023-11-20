#pragma once
#include "rendering/mesh.h"
#include "interfaces/iContainer.h"

class MeshContainer : public IContainer<Mesh>
{
protected:
	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	const std::pair<std::string, Mesh*> ReadItem(std::ifstream& file) override
	{
		return std::pair<std::string, Mesh*>("", nullptr);
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
		file << "material " << (item->GetMaterial() != nullptr ? item->GetMaterial()->name : "none") << std::endl;
		file << "enabled " << (int)item->IsEnabled() << std::endl;

		// Write all vertices
		const auto& verts = item->GetVertices();
		file << "vertices " << verts.size() << std::endl;
		file << "#StartVerts" << std::endl;
		for (unsigned int i = 0; i < verts.size(); i++)
		{
			Vertex vert = verts[i];
			file << i << " " << Vec3ToString(verts[i].pos) << " ";
			file << Vec3ToString(verts[i].normal) << " ";
			file << Vec3ToString(verts[i].tangent) << " ";
			file << Vec2ToString(verts[i].texCoords) << std::endl;
		}
		file << "#EndVerts" << std::endl;
		file << std::endl;

		// Write all indices
		const auto& inds = item->GetIndices();
		file << "indices " << inds.size() << std::endl;
		file << "#StartInds" << std::endl;
		unsigned int i = 0;
		for (unsigned int i = 0; i < inds.size(); i++)
		{
			// Group together indices by 6s (2 tris)
			if (i != 0 && i % 6 == 0)
				file << std::endl;
			file << inds[i] << " ";
		}
		file << "#EndInds" << std::endl;
		file << std::endl;
	}
};