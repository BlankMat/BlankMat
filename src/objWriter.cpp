#include "objWriter.h"

void WriteObjToFile(Scene* scene, std::string location, std::string fileName)
{
	// Avoid overwriting files
	std::string objLoc = location + fileName + ".obj";
	int counter = 0;
	while (FileExists(objLoc)) {
		objLoc = location + fileName + "_" + std::to_string(counter) + ".obj";
		counter++;
	}

	Mesh* mesh = scene->GetMeshes()->GetAll().begin()->second;
	auto verts = mesh->GetVerts();
	auto faces = mesh->GetFaces();

	// Open the object
	// ---------------
	std::ofstream objFile;
	objFile.open(objLoc);

	// Write header
	// ------------
	objFile << "##\n";
	objFile << "## " << fileName << ".obj\n";
	objFile << "##\n";
	objFile << "## This file was generated using BlankMat\n";
	objFile << "## created by Kristian Kolehmainen, Maia Gervasone, William Liu, and Elias Estrada.\n";
	objFile << "## There is no copyright attributed to this file.\n";
	objFile << "##\n";
	objFile << "\n";

	// Write material
	// --------------
	if (scene->GetMats()->IsNonDefault()) {
		objFile << "mtllib " << fileName << ".mtl\n";
		WriteMtlToFile(scene->GetMats(), location, fileName);
	}

	// Write vertices
	// --------------
	objFile << "g default\n";
	int vertsSize = (int)verts.size();
	bool anyTexts = false;
	for (int i = 0; i < vertsSize; i++)
	{
		// Search for any texture information
		if (verts[i].texture != glm::vec3())
			anyTexts = true;

		glm::vec3 pos = verts[i].pos;
		objFile << "v " << RoundToPrecision(pos.x, 6) << " " << RoundToPrecision(pos.y, 6) << " " << RoundToPrecision(pos.z, 6) << "\n";
	}
	objFile << "# " << vertsSize << " vertices\n\n";

	// Write vertex normals
	// --------------------
	for (int i = 0; i < vertsSize; i++)
	{
		glm::vec3 norm = verts[i].normal;
		objFile << "vn " << RoundToPrecision(norm.x, 6) << " " << RoundToPrecision(norm.y, 6) << " " << RoundToPrecision(norm.z, 6) << "\n";
	}
	objFile << "# " << vertsSize << " vertex normals\n\n";

	// Write vertex textures only if needed
	// ------------------------------------
	int vertTextSize = 0;
	if (anyTexts) {
		vertTextSize = vertsSize;
		for (int i = 0; i < vertsSize; i++)
		{
			glm::vec3 text = verts[i].texture;
			objFile << "vt " << RoundToPrecision(text.x, 6) << " " << RoundToPrecision(text.y, 6) << " " << RoundToPrecision(text.z, 6) << "\n";
		}
	}
	objFile << "# " << vertTextSize << " texture vertices\n\n";

	// Write faces
	// -----------
	int facesSize = (int)faces.size();
	std::string curMat = "";
	for (int i = 0; i < facesSize; i++)
	{
		std::string tempMat = faces[i].mat;
		auto faceVerts = faces[i].vertices;

		// Write material info if there is a change in materials
		if (tempMat != curMat) {
			curMat = tempMat;
			objFile << "g " << curMat << "\n";
			objFile << "usemtl " << curMat << "\n";
		}

		// Write face
		objFile << "f ";
		for (int fi = 0; fi < faceVerts.size(); fi++) {
			// Store texture data if it is used
			if (anyTexts) {
				objFile << (faceVerts[fi] + 1) << "/" << (faceVerts[fi] + 1) << "/" << (faceVerts[fi] + 1) << " ";
			}
			// Only store normal and vertex data if there are no textures
			else {
				objFile << (faceVerts[fi] + 1) << "//" << (faceVerts[fi] + 1) << " ";
			}
		}
		objFile << "\n";
	}
	objFile << "# " << facesSize << " faces\n\n";

	// Write material groups
	// ---------------------

	// Close file
	// ----------
	objFile.close();
	std::cout << "Wrote file: " << objLoc << std::endl;
}

void WriteMtlToFile(MaterialStorage* mat, std::string location, std::string fileName)
{
	std::string mtlLoc = location + fileName + ".mtl";
	auto mats = mat->GetAll();
	// Open the object
	std::ofstream mtlFile;
	mtlFile.open(mtlLoc);

	// Write header
	mtlFile << "##\n";
	mtlFile << "## " << fileName << ".mtl\n";
	mtlFile << "##\n";
	mtlFile << "## This file was generated using BlankMat\n";
	mtlFile << "## created by Kristian Kolehmainen, Maia Gervasone, William Liu, and Elias Estrada.\n";
	mtlFile << "## There is no copyright attributed to this file.\n";
	mtlFile << "##\n\n";

	// Write all materials
	for (auto iter = mats.begin(); iter != mats.end(); iter++) {
		// Don't write the default mat to files
		if (iter->first == "default")
			continue;

		// Always write the material name, ka, and kd
		mtlFile << "newmtl " << iter->first << "\n";
		mtlFile << "Ka " << iter->second->ka.r << " " << iter->second->ka.g << " " << iter->second->ka.b << "\n";
		mtlFile << "Kd " << iter->second->kd.r << " " << iter->second->kd.g << " " << iter->second->kd.b << "\n";

		// Only write non-default values for other mtl elements
		if (iter->second->ks != glm::vec3(0, 0, 0))
			mtlFile << "Ks " << iter->second->ks.r << " " << iter->second->ks.g << " " << iter->second->ks.b << "\n";

		if (iter->second->mapkd != "")
			mtlFile << "map_Kd " << iter->second->mapkd << "\n";

		if (iter->second->d != 1.0f)
			mtlFile << "d " << iter->second->d << "\n";

		if (iter->second->ni != 1.0f)
			mtlFile << "ni " << iter->second->ni << "\n";

		if (iter->second->ns != 0.0f)
			mtlFile << "ns " << iter->second->ns << "\n";

		mtlFile << "\n";
	}

	// Close file
	mtlFile.close();
}