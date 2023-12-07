#include "files/sceneWriter.h"
#include "timer.h"
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

std::string SceneWriter::GetAssimpFormatID(const Assimp::Exporter& exporter, const std::string& path)
{
	// Find requested file format
	std::string format = GetFileFormat(path);
	for (size_t i = 0; i < exporter.GetExportFormatCount(); i++)
		if (exporter.GetExportFormatDescription(i)->fileExtension == format)
			return exporter.GetExportFormatDescription(i)->id;

	// Default to obj
	return "obj";
}

// Processes the given mesh
unsigned int SceneWriter::ProcessAssimpMesh(Mesh* sourceMesh, aiMesh*& newMesh, std::vector<aiMesh*>& outMeshWriteList, std::vector<aiMaterial*>& outMatList)
{
	// Gather all vertex info from mesh into vectors
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	// Write vertices by index
	const std::vector<Vertex>& meshVerts = sourceMesh->GetVertices();
	const std::vector<unsigned int>& meshInds = sourceMesh->GetIndices();
	for (unsigned int i = 0; i < meshInds.size(); i++)
	{
		Vertex vert = meshVerts[meshInds[i]];
		glm::vec3 transformPos = glm::vec3(sourceMesh->GetSelfModelMatrix() * glm::vec4(vert.pos, 1.0f));
		vertices.push_back(transformPos);
		normals.push_back(vert.normal);
		tangents.push_back(vert.tangent);
		uvs.push_back(vert.texCoords);
	}

	// Copy vertices
	const auto& outVertices = vertices;
	const unsigned int numVerts = (unsigned int)outVertices.size();
	const unsigned int numFaces = (unsigned int)(outVertices.size() / 3);

	// Init mesh properties
	newMesh->mVertices = new aiVector3D[numVerts];
	newMesh->mNormals = new aiVector3D[numVerts];
	newMesh->mTangents = new aiVector3D[numVerts];
	newMesh->mNumVertices = numVerts;
	
	newMesh->mTextureCoords[0] = new aiVector3D[numVerts];
	newMesh->mNumUVComponents[0] = numVerts;

	newMesh->mFaces = new aiFace[numFaces];
	newMesh->mNumFaces = numFaces;

	// Get material from the list
	newMesh->mMaterialIndex = 0;
	if (sourceMesh->GetMaterial() != nullptr)
	{
		for (unsigned int i = 0; i < (unsigned int)outMatList.size(); i++)
		{
			aiString name;
			outMatList[i]->Get(AI_MATKEY_NAME, name);
			if (name.C_Str() == sourceMesh->GetMaterial()->GetScopedName().c_str())
			{
				newMesh->mMaterialIndex = i;
				break;
			}
		}
	}

	// Fill mesh properties from vertices
	unsigned int j = 0;
	for (auto iter = outVertices.begin(); iter != outVertices.end(); ++iter)
	{
		newMesh->mVertices[iter - outVertices.begin()] = Vec3ToAssimp(outVertices[j]);
		newMesh->mNormals[iter - outVertices.begin()] = Vec3ToAssimp(normals[j]);
		newMesh->mTangents[iter - outVertices.begin()] = Vec3ToAssimp(tangents[j]);
		newMesh->mTextureCoords[0][iter - outVertices.begin()] = Vec3ToAssimp(glm::vec3(uvs[j], 0));
		j++;
	}

	// Fill face properties from indices
	unsigned int k = 0;
	for (unsigned int i = 0; i < numFaces; i++)
	{
		aiFace& face = newMesh->mFaces[i];
		face.mIndices = new unsigned int[3];
		face.mNumIndices = 3;

		face.mIndices[0] = k;
		face.mIndices[1] = k+1;
		face.mIndices[2] = k+2;
		k += 3;
	}

	// Push new mesh to mesh list and return its index (max-1)
	outMeshWriteList.push_back(newMesh);
	return (unsigned int)(outMeshWriteList.size() - 1);
}

// Processes all materials of the scene
void SceneWriter::ProcessAssimpMaterials(Scene* sourceScene, aiScene*& newScene, std::vector<aiMaterial*>& outMatList, std::vector<aiTexture*>& outTextureList)
{
	const auto& materials = sourceScene->GetMaterials()->Data();
	for (auto iter = materials.begin(); iter != materials.end(); ++iter)
	{
		// Don't write null materials
		Material* mat = iter->second;
		if (mat == nullptr || mat->IsInternal())
			continue;

		aiMaterial* newMat = new aiMaterial();
		aiString name;
		name.Set(mat->GetScopedName().c_str());
		newMat->AddProperty(&name, AI_MATKEY_NAME);

		const aiColor3D diffuse = ColorToAssimp(mat->mKD);
		newMat->AddProperty(&diffuse, 1, AI_MATKEY_COLOR_DIFFUSE);

		const aiColor3D ambient = ColorToAssimp(mat->mKA);
		newMat->AddProperty(&ambient, 1, AI_MATKEY_COLOR_AMBIENT);

		const aiColor3D specular = ColorToAssimp(mat->mKS);
		newMat->AddProperty(&specular, 1, AI_MATKEY_COLOR_SPECULAR);

		const aiColor3D emissive = ColorToAssimp(mat->mKE);
		newMat->AddProperty(&emissive, 1, AI_MATKEY_COLOR_EMISSIVE);

		newMat->AddProperty(&mat->mNS, 1, AI_MATKEY_SHININESS);
		newMat->AddProperty(&mat->mNI, 1, AI_MATKEY_REFRACTI);
		newMat->AddProperty(&mat->mD, 1, AI_MATKEY_OPACITY);

		if (mat->mMapKD != nullptr && !mat->mMapKD->IsInternal())
		{
			aiString kd;
			kd.Set(mat->mMapKD->mFile.c_str());
			newMat->AddProperty(&kd, AI_MATKEY_TEXTURE_DIFFUSE(0));
		}

		if (mat->mMapKA != nullptr && !mat->mMapKA->IsInternal())
		{
			aiString ka;
			ka.Set(mat->mMapKA->mFile.c_str());
			newMat->AddProperty(&ka, AI_MATKEY_TEXTURE_AMBIENT(0));
		}

		if (mat->mMapKS != nullptr && !mat->mMapKS->IsInternal())
		{
			aiString ks;
			ks.Set(mat->mMapKS->mFile.c_str());
			newMat->AddProperty(&ks, AI_MATKEY_TEXTURE_SPECULAR(0));
		}

		if (mat->mMapBump != nullptr && !mat->mMapBump->IsInternal())
		{
			aiString bump;
			bump.Set(mat->mMapBump->mFile.c_str());
			newMat->AddProperty(&bump, AI_MATKEY_TEXTURE_HEIGHT(0));
		}

		if (mat->mMapNS != nullptr && !mat->mMapNS->IsInternal())
		{
			aiString ns;
			ns.Set(mat->mMapNS->mFile.c_str());
			newMat->AddProperty(&ns, AI_MATKEY_TEXTURE_DISPLACEMENT(0));
		}

		if (mat->mMapD != nullptr && !mat->mMapD->IsInternal())
		{
			aiString d;
			d.Set(mat->mMapD->mFile.c_str());
			newMat->AddProperty(&d, AI_MATKEY_TEXTURE_OPACITY(0));
		}

		outMatList.push_back(newMat);
	}
}

// Processes all textures of the scene
void SceneWriter::ProcessAssimpTextures(Scene* sourceScene, const std::string& path, std::vector<aiTexture*>& outTextureList)
{
	const auto& textures = sourceScene->GetTextures()->Data();
	for (auto iter = textures.begin(); iter != textures.end(); ++iter)
	{
		// Dont' write internal or null textures
		Texture* texture = iter->second;
		if (texture == nullptr || texture->IsInternal())
			continue;

		std::string dir = GetDirectory(path);
		Texture::CopyTexture(texture->mFile, texture->mDir, dir);
	}
}

// Recursively processes the meshes in the given node and all its children
void SceneWriter::ProcessAssimpNode(Scene* sourceScene, Node* sourceNode, aiNode*& newNode, aiScene*& newScene, std::vector<aiMesh*>& outMeshWriteList, std::vector<aiMaterial*>& outMatList)
{
	std::cout << " - Writing node " << sourceNode->GetScopedName() << std::endl;

	// Process all the node's meshes (if any)
	unsigned int meshCount = sourceNode->MeshCount();
	newNode->mMeshes = new unsigned int[meshCount];
	for (unsigned int i = 0; i < meshCount; ++i)
	{
		aiMesh* newMesh = new aiMesh();
		newNode->mMeshes[i] = ProcessAssimpMesh(sourceNode->GetMesh(i), newMesh, outMeshWriteList, outMatList);
	}
	newNode->mNumMeshes = meshCount;

	// Process each of the children
	unsigned int childCount = sourceNode->ChildCount();
	aiNode** tempChildren = new aiNode*[childCount];
	for (unsigned int i = 0; i < childCount; ++i)
	{
		// If the node is null, subtract it from the count
		Node* child = sourceNode->GetChild(i);
		if (child == nullptr)
		{
			childCount--;
			continue;
		}

		aiNode* childNode = new aiNode(child->GetScopedName());
		childNode->mParent = childNode;
		childNode->mTransformation = Mat4ToAssimp(child->GetSelfModelMatrix());
		tempChildren[i] = childNode;

		ProcessAssimpNode(sourceScene, child, childNode, newScene, outMeshWriteList, outMatList);
	}
	// Add children to node
	newNode->addChildren(childCount, tempChildren);
	newNode->mNumChildren = childCount;
}

void SceneWriter::GenerateAssimpScene(Scene* sourceScene, aiScene*& newScene, const std::string& path)
{
	// Initialize storage for scene components
	std::vector<aiMesh*> meshList;
	std::vector<aiMaterial*> matList;
	std::vector<aiTexture*> textureList;

	newScene->mRootNode = new aiNode();
	ProcessAssimpTextures(sourceScene, path, textureList);
	ProcessAssimpMaterials(sourceScene, newScene, matList, textureList);
	ProcessAssimpNode(sourceScene, sourceScene->GetRootNode(), newScene->mRootNode, newScene, meshList, matList);

	// Write mesh list to scene
	unsigned int meshCount = (unsigned int)meshList.size();
	newScene->mMeshes = new aiMesh*[meshCount];
	for (unsigned int i = 0; i < meshCount; i++)
		newScene->mMeshes[i] = meshList[i];
	newScene->mNumMeshes = meshCount;

	// Write material list to scene
	unsigned int matCount = (unsigned int)matList.size();
	newScene->mMaterials = new aiMaterial*[matCount];
	for (unsigned int i = 0; i < matCount; i++)
		newScene->mMaterials[i] = matList[i];
	newScene->mNumMaterials = matCount;

	// Write texture list to scene
	unsigned int textureCount = (unsigned int)textureList.size();
	newScene->mTextures = new aiTexture*[textureCount];
	for (unsigned int i = 0; i < textureCount; i++)
		newScene->mTextures[i] = textureList[i];
	newScene->mNumTextures = textureCount;

	// Mark lights, cameras, skeletons, and animations as empty
	newScene->mAnimations = nullptr;
	newScene->mSkeletons = nullptr;
	newScene->mLights = nullptr;
	newScene->mCameras = nullptr;
	newScene->mPrivate = nullptr;
}

void SceneWriter::SaveAssimpScene(Scene* scene, const std::string& path)
{
	double startTime = Timer::Start();
	std::cout << "Saving scene to file " << path << std::endl;

	aiScene* assimpScene = new aiScene();
	Assimp::Exporter exporter = Assimp::Exporter();
	std::string format = GetAssimpFormatID(exporter, path);

	GenerateAssimpScene(scene, assimpScene, path);
	aiReturn exportReturn = exporter.Export(assimpScene, format, path);

	// If exporting fails, print
	if (exportReturn != AI_SUCCESS)
	{
		std::cout << "ERROR::ASSIMP::EXPORTFAILED: " << std::endl;
	}

	Timer::Time(startTime, "Saved scene");
}

void SceneWriter::WriteBlankMatItem(IWritable* item, std::ofstream& file, const std::string& label)
{
	double startTime = Timer::Start();
	file << "# " << label << std::endl;
	item->Write(file);
	file << std::endl;
	Timer::Time(startTime, "Wrote " + std::to_string(item->WriteCount()) + " " + label);
}

void SceneWriter::SaveBlankMatScene(Scene* scene, const std::string& path)
{
	double startTime = Timer::Start();
	std::cout << "Saving scene to file " << path << std::endl;
	std::ofstream file(path);
	try
	{
		WriteBlankMatItem(scene->GetRootNode(), file, "Nodes");
		WriteBlankMatItem(scene->GetMeshes(), file, "Meshes");
		WriteBlankMatItem(scene->GetMaterials(), file, "Materials");
		WriteBlankMatItem(scene->GetTextures(), file, "Textures");
		WriteBlankMatItem(scene->GetCameras(), file, "Cameras");
		WriteBlankMatItem(scene->GetLights(), file, "Lights");

		// Write textures to save location
		const auto& textures = scene->GetTextures()->Data();
		for (auto iter = textures.begin(); iter != textures.end(); ++iter)
		{
			// Dont' write internal or null textures
			Texture* texture = iter->second;
			if (texture == nullptr || texture->IsInternal())
				continue;

			std::string dir = GetDirectory(path);
			Texture::CopyTexture(texture->mFile, texture->mDir, dir);
		}
		Timer::Time(startTime, "Saved scene");
	}
	catch (std::exception const& e)
	{
		std::cout << "ERROR::SAVESCENE::WRITE " << e.what() << std::endl;
	}
	file.close();
}

void SceneWriter::SaveScene(Scene* scene, const std::string& path)
{
	// Get extension
	std::string ext = GetFileExtension(path);
	if (ext == BLANK)
	{
		SaveBlankMatScene(scene, path);
	}
	// Validate that the export extension is supported
	else if (ext == DAE || ext == OBJ || ext == STL || ext == PLY)
	{
		SaveAssimpScene(scene, path);
	}
	else
	{
		std::cout << "ERROR::SAVESCENE::INVALID_FORMAT: File format " << ext << " not supported or not recognized." << std::endl;
	}
}