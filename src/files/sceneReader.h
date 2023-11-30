#pragma once
#include "glIncludes.h"
#include "timer.h"
#include "rendering/scene.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>

class SceneReader
{
private:
	// Recursively processes the meshes in the given node and all its children
	static void ProcessAssimpNode(Scene* scene, Node* node, const aiNode* assimpNode, const aiScene* assimpScene)
	{
		std::cout << " - Reading node " << assimpNode->mName.C_Str() << std::endl;
		// Process all the node's meshes (if any)
		for (unsigned int i = 0; i < assimpNode->mNumMeshes; i++)
		{
			aiMesh* mesh = assimpScene->mMeshes[assimpNode->mMeshes[i]];
			Mesh* newMesh = ProcessAssimpMesh(scene, mesh, assimpScene);
			scene->AddMesh(newMesh, node);
		}
		// Process each of the children
		for (unsigned int i = 0; i < assimpNode->mNumChildren; i++)
		{
			Node* childNode = new Node(node, assimpNode->mChildren[i]->mName.C_Str());
			node->AddChild(childNode);
			ProcessAssimpNode(scene, childNode, assimpNode->mChildren[i], assimpScene);
		}
	}

	// Process the vertices, indices, and textures of the given mesh
	static Mesh* ProcessAssimpMesh(Scene* scene, const aiMesh* assimpMesh, const aiScene* assimpScene)
	{
		// Process each part of the mesh
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		ProcessAssimpVertices(assimpMesh, vertices);
		ProcessAssimpIndices(vertices, indices, assimpMesh);
		Material* newMaterial = ProcessAssimpMaterial(scene, assimpMesh, assimpScene);

		// Construct mesh
		return new Mesh(assimpMesh->mName.C_Str(), vertices, indices, newMaterial);
	}

	// Processes the vertices of the mesh, emplacing them into the list given
	static void ProcessAssimpVertices(const aiMesh* mesh, std::vector<Vertex>& outVertices)
	{
		// Process each vertex
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			glm::vec3 pos = Vec3FromAssimp(mesh->mVertices[i]);
			glm::vec3 normal = Vec3FromAssimp(mesh->mNormals[i]);
			glm::vec2 texCoords = (mesh->mTextureCoords[0]) ? Vec2FromAssimp(mesh->mTextureCoords[0][i]) : glm::vec2();
			glm::vec3 tangent = glm::vec3(0.0f);
			if (mesh->mTangents != nullptr && mesh->mTangents->Length() > 0)
			{
				tangent = Vec3FromAssimp(mesh->mTangents[i]);
			}
			outVertices.push_back(Vertex(pos, normal, texCoords, tangent));
		}
	}

	// Processes the indices of the mesh
	static void ProcessAssimpIndices(const std::vector<Vertex>& vertices, std::vector<unsigned int>& outIndices, const aiMesh* assimpMesh)
	{
		// Process each index
		for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++)
		{
			if (assimpMesh->mFaces[i].mNumIndices > 0 && vertices[assimpMesh->mFaces[i].mIndices[0]].tangent == glm::vec3(0.0f))
			{
				// If vertex does not have tangents, calculate them
				Vertex vertex1 = vertices[assimpMesh->mFaces[i].mIndices[0]];
				Vertex vertex2 = vertices[assimpMesh->mFaces[i].mIndices[1]];
				Vertex vertex3 = vertices[assimpMesh->mFaces[i].mIndices[2]];
				glm::vec3 edge1 = vertex2.pos - vertex1.pos;
				glm::vec3 edge2 = vertex3.pos - vertex1.pos;
				glm::vec2 deltaUV1 = vertex2.texCoords - vertex1.texCoords;
				glm::vec2 deltaUV2 = vertex3.texCoords - vertex1.texCoords;
				glm::vec3 tangent = glm::vec3(0.0f);

				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
				tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

				vertex1.tangent = tangent;
				vertex2.tangent = tangent;
				vertex3.tangent = tangent;
			}
			for (unsigned int j = 0; j < assimpMesh->mFaces[i].mNumIndices; j++)
			{
				outIndices.push_back(assimpMesh->mFaces[i].mIndices[j]);
			}
		}
	}

	// Processes the material of the mesh
	static Material* ProcessAssimpMaterial(Scene* scene, const aiMesh* assimpMesh, const aiScene* assimpScene)
	{
		// If there are no textures, use default material
		if (assimpMesh->mMaterialIndex >= assimpScene->mNumMaterials)
			return scene->GetDefaultMaterial();

		// Process each material's diffuse and specular maps
		aiMaterial* material = assimpScene->mMaterials[assimpMesh->mMaterialIndex];

		// If the material already exists, return it
		Material* newMaterial = scene->GetMaterial(material->GetName().C_Str());
		if (newMaterial != nullptr)
			return newMaterial;

		// 1. diffuse maps
		std::vector<Texture*> diffuseMaps = LoadMaterialTextures(scene, material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		if (diffuseMaps.empty())
			diffuseMaps.push_back(scene->GetTexture("default_diffuse"));
		// 2. ambient maps
		std::vector<Texture*> ambientMaps = LoadMaterialTextures(scene, material, aiTextureType_AMBIENT, TextureType::AMBIENT);
		if (ambientMaps.empty())
		{
			if (!diffuseMaps.empty())
			{
				Texture* ambientTexture = new Texture(diffuseMaps[0]->id, TextureType::AMBIENT, diffuseMaps[0]->path, diffuseMaps[0]->name);
				scene->AddTexture(ambientTexture->name, ambientTexture);
				ambientMaps.push_back(ambientTexture);
			}
			else
			{
				ambientMaps.push_back(scene->GetTexture("default_ambient"));
			}
		}
		// 3. specular maps
		std::vector<Texture*> specularMaps = LoadMaterialTextures(scene, material, aiTextureType_SPECULAR, TextureType::SPECULAR);
		if (specularMaps.empty())
			specularMaps.push_back(scene->GetTexture("default_specular"));
		// 4. normal maps
		std::vector<Texture*> normalMaps = LoadMaterialTextures(scene, material, aiTextureType_HEIGHT, TextureType::NORMAL);
		if (normalMaps.empty())
			normalMaps.push_back(scene->GetTexture("default_normal"));
		// 5. height maps
		std::vector<Texture*> heightMaps = LoadMaterialTextures(scene, material, aiTextureType_DISPLACEMENT, TextureType::HEIGHT);
		if (heightMaps.empty())
			heightMaps.push_back(scene->GetTexture("default_height"));
		// 6. alpha maps
		std::vector<Texture*> alphaMaps = LoadMaterialTextures(scene, material, aiTextureType_OPACITY, TextureType::ALPHA);
		if (alphaMaps.empty())
			alphaMaps.push_back(scene->GetTexture("default_alpha"));

		// Read properties into material
		aiColor3D diffuse;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		glm::vec3 kd = Vec3FromAssimp(diffuse);

		aiColor3D ambient;
		material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		glm::vec3 ka = Vec3FromAssimp(ambient);

		aiColor3D specular;
		material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		glm::vec3 ks = Vec3FromAssimp(specular);

		aiColor3D emissive;
		material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
		glm::vec3 ke = Vec3FromAssimp(emissive);

		float ns;
		material->Get(AI_MATKEY_SHININESS, ns);

		float ni;
		material->Get(AI_MATKEY_REFRACTI, ni);

		float d;
		material->Get(AI_MATKEY_OPACITY, d);

		// Make material out of loaded textures
		newMaterial = new Material(material->GetName().C_Str(), diffuseMaps, ambientMaps, specularMaps, normalMaps, heightMaps, alphaMaps,
			ka, kd, ks, ns, ni, d, ke);
		scene->AddMaterial(material->GetName().C_Str(), newMaterial);
		return newMaterial;
	}

	// Loads material textures
	static std::vector<Texture*> LoadMaterialTextures(Scene* scene, const aiMaterial* assimpMaterial, aiTextureType assimpTextureType, TextureType type)
	{
		std::vector<Texture*> textures;
		for (unsigned int i = 0; i < assimpMaterial->GetTextureCount(assimpTextureType); i++)
		{
			aiString str;
			assimpMaterial->GetTexture(assimpTextureType, i, &str);
			std::string path = str.C_Str();
			std::string name = path.substr(0, path.find_last_of('.'));
			std::string textureName = scene->GetName() + "_" + name;

			// Check if texture is loaded already
			Texture* texture = scene->GetTexture(textureName);
			if (texture != nullptr)
			{
				textures.push_back(texture);
				continue;
			}

			// if texture hasn't been loaded already, load it
			texture = new Texture(type, scene->GetDirectory(), path, textureName);
			textures.push_back(texture);
			scene->AddTexture(textureName, texture);
			std::cout << "  - Loaded texture " << textureName << std::endl;
		}
		return textures;
	}

	static void ReadBlankMatItem(IWritable* container, bool replace, std::ifstream& file, const std::string& label)
	{
		std::cout << "Reading " << label << std::endl;
		double startTime = Timer::Start();
		container->Read(file, replace);
		Timer::Time(startTime, "Read " + std::to_string(container->WriteCount()) + " " + label);
	}

	static void ReadAssimpScene(Scene* scene, const std::string& path, bool replace)
	{
		double loadStartTime = Timer::Start();
		std::cout << "Reading model from file " << path << std::endl;
		Assimp::Importer importer;
		const aiScene* assimpScene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_FindInvalidData |
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_FixInfacingNormals | aiProcess_CalcTangentSpace);

		if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		size_t lastSlash = path.find_last_of('/') + 1;
		size_t lastPeriod = path.find_last_of('.');
		scene->SetDirectory(path.substr(0, lastSlash));
		scene->SetName(path.substr(lastSlash, lastPeriod - lastSlash));

		// Get root node
		Node* rootNode = scene->GetRootNode();
		if (rootNode == nullptr)
		{
			rootNode = new Node(nullptr, "root");
			scene->SetRootNode(rootNode);
		}
		Node* sceneRootNode = new Node(rootNode, scene->GetName());

		// Process model
		rootNode->AddChild(sceneRootNode);
		ProcessAssimpNode(scene, sceneRootNode, assimpScene->mRootNode, assimpScene);
		rootNode->SetParentModelMatrix(glm::mat4(1.0f));

		Timer::Time(loadStartTime, "Read model from file " + path + " successfully");
		std::cout << "Project directory is " << scene->GetDirectory() << std::endl;
	}

	static void ReadBlankMatScene(Scene* scene, const std::string& path, bool replace)
	{
		double startTime = Timer::Start();
		std::cout << "Reading model from file " << path << std::endl;
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cout << "ERROR::READSCENE::FILE_DOES_NOT_EXIST file " << path << " could not be opened." << std::endl;
			return;
		}

		try
		{
			// Clears the scene of everything
			if (replace)
				scene->Clear();

			// Read all items
			ReadBlankMatItem(scene->GetRootNode(), replace, file, "Nodes");
			ReadBlankMatItem(scene->GetMeshes(), replace, file, "Meshes");
			ReadBlankMatItem(scene->GetMaterials(), replace, file, "Materials");
			ReadBlankMatItem(scene->GetTextures(), replace, file, "Textures");
			ReadBlankMatItem(scene->GetCameras(), replace, file, "Cameras");
			ReadBlankMatItem(scene->GetLights(), replace, file, "Lights");

			// Reconstruct relationships between items
			scene->GetMaterials()->LoadTextures(scene->GetTextures());
			scene->GetMeshes()->LoadMaterials(scene->GetMaterials());
			scene->GetRootNode()->LoadMeshes(scene->GetMeshes());
			scene->GetRootNode()->RecalcMatrices();
			scene->UpdateRenderList();

			Timer::Time(startTime, "Read scene from file " + path + " successfully");
		}
		catch (std::exception const& e)
		{
			std::cout << "ERROR::READSCENE::READ " << e.what() << std::endl;
		}
		file.close();
	}
public:
	static void ReadScene(Scene* scene, const std::string& path, bool replace)
	{
		// Get extension
		size_t lastPeriod = path.find_last_of('.');
		std::string ext = "";
		if (lastPeriod < path.length())
			ext = path.substr(lastPeriod);

		// Disable rendering until reading is done
		scene->GetState()->shouldRender = false;
		if (ext == ".blank")
		{
			ReadBlankMatScene(scene, path, replace);
		}
		else
		{
			ReadAssimpScene(scene, path, replace);
		}
		scene->GetState()->shouldRender = true;
	}
};