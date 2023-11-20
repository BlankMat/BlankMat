#pragma once
#include "glIncludes.h"
#include "rendering/scene.h"
#include "timer.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelReader
{
private:
	// Recursively processes the meshes in the given node and all its children
	static void ProcessNode(Scene* scene, Node* node, const aiNode* assimpNode, const aiScene* assimpScene)
	{
		std::cout << " - Reading node " << assimpNode->mName.C_Str() << std::endl;
		// Process all the node's meshes (if any)
		for (unsigned int i = 0; i < assimpNode->mNumMeshes; i++)
		{
			aiMesh* mesh = assimpScene->mMeshes[assimpNode->mMeshes[i]];
			Mesh* newMesh = ProcessMesh(scene, mesh, assimpScene);
			scene->AddMesh(newMesh, node);
		}
		// Process each of the children
		for (unsigned int i = 0; i < assimpNode->mNumChildren; i++)
		{
			Node* childNode = new Node(node, assimpNode->mChildren[i]->mName.C_Str());
			node->AddChild(childNode);
			ProcessNode(scene, childNode, assimpNode->mChildren[i], assimpScene);
		}
	}

	// Process the vertices, indices, and textures of the given mesh
	static Mesh* ProcessMesh(Scene* scene, const aiMesh* mesh, const aiScene* assimpScene)
	{
		// Process each part of the mesh
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		ProcessVertices(mesh, vertices);
		ProcessIndices(vertices, indices, mesh);
		Material* newMaterial = ProcessMaterial(scene, mesh, assimpScene);

		// Construct mesh
		return new Mesh(mesh->mName.C_Str(), vertices, indices, newMaterial);
	}

	// Processes the vertices of the mesh, emplacing them into the list given
	static void ProcessVertices(const aiMesh* mesh, std::vector<Vertex>& outVertices)
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
	static void ProcessIndices(const std::vector<Vertex>& vertices, std::vector<unsigned int>& outIndices, const aiMesh* mesh)
	{
		// Process each index
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			if (mesh->mFaces[i].mNumIndices > 0 && vertices[mesh->mFaces[i].mIndices[0]].tangent == glm::vec3(0.0f))
			{
				// If vertex does not have tangents, calculate them
				Vertex vertex1 = vertices[mesh->mFaces[i].mIndices[0]];
				Vertex vertex2 = vertices[mesh->mFaces[i].mIndices[1]];
				Vertex vertex3 = vertices[mesh->mFaces[i].mIndices[2]];
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
			for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			{
				outIndices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}
	}

	// Processes the material of the mesh
	static Material* ProcessMaterial(Scene* scene, const aiMesh* mesh, const aiScene* assimpScene)
	{
		// If there are no textures, use default material
		if (mesh->mMaterialIndex >= assimpScene->mNumMaterials)
			return scene->GetMaterial("default");

		// Process each material's diffuse and specular maps
		aiMaterial* material = assimpScene->mMaterials[mesh->mMaterialIndex];

		// If the material already exists, return it
		Material* newMaterial = scene->GetMaterial(material->GetName().C_Str());
		if (newMaterial != nullptr)
			return newMaterial;

		// 1. diffuse maps
		std::vector<Texture*> diffuseMaps = LoadMaterialTextures(scene, material, aiTextureType_DIFFUSE, "texture_diffuse");
		if (diffuseMaps.empty())
			diffuseMaps.push_back(scene->GetTexture("default_diffuse"));
		// 2. ambient maps
		std::vector<Texture*> ambientMaps = LoadMaterialTextures(scene, material, aiTextureType_AMBIENT, "texture_ambient");
		if (ambientMaps.empty())
		{
			if (!diffuseMaps.empty())
			{
				Texture* ambientTexture = new Texture(diffuseMaps[0]->id, "texture_ambient", diffuseMaps[0]->path, diffuseMaps[0]->name);
				scene->AddTexture(ambientTexture->name, ambientTexture);
				ambientMaps.push_back(ambientTexture);
			}
			else
			{
				ambientMaps.push_back(scene->GetTexture("default_ambient"));
			}
		}
		// 3. specular maps
		std::vector<Texture*> specularMaps = LoadMaterialTextures(scene, material, aiTextureType_SPECULAR, "texture_specular");
		if (specularMaps.empty())
			specularMaps.push_back(scene->GetTexture("default_specular"));
		// 4. normal maps
		std::vector<Texture*> normalMaps = LoadMaterialTextures(scene, material, aiTextureType_HEIGHT, "texture_normal");
		if (normalMaps.empty())
			normalMaps.push_back(scene->GetTexture("default_normal"));
		// 5. height maps
		std::vector<Texture*> heightMaps = LoadMaterialTextures(scene, material, aiTextureType_DISPLACEMENT, "texture_height");
		if (heightMaps.empty())
			heightMaps.push_back(scene->GetTexture("default_height"));
		// 6. alpha maps
		std::vector<Texture*> alphaMaps = LoadMaterialTextures(scene, material, aiTextureType_OPACITY, "texture_alpha");
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
	static std::vector<Texture*> LoadMaterialTextures(Scene* scene, const aiMaterial* mat, aiTextureType type, const std::string& typeName)
	{
		std::vector<Texture*> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
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
			texture = new Texture(typeName, scene->GetDirectory(), path, textureName);
			textures.push_back(texture);
			scene->AddTexture(textureName, texture);
			std::cout << "  - Loaded texture " << textureName << std::endl;
		}
		return textures;
	}
public:
	// Loads the scene at the given path
	static void LoadModel(Scene* scene, const std::string& path, const glm::vec3& startPos = glm::vec3(0.0f), const glm::vec3& startRot = glm::vec3(0.0f), const glm::vec3& startScale = glm::vec3(1.0f))
	{
		double loadStartTime = glfwGetTime();
		std::cout << "Reading model from file " << path << std::endl;
		Assimp::Importer importer;
		const aiScene* assimpScene = importer.ReadFile(path,
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
		Node* sceneRootNode = new Node(rootNode, scene->GetName(), startPos, startRot, startScale);

		// Process model
		rootNode->AddChild(sceneRootNode);
		ProcessNode(scene, sceneRootNode, assimpScene->mRootNode, assimpScene);
		rootNode->SetParentModelMatrix(glm::mat4(1.0f));

		Timer::Time(loadStartTime, "Read model from file " + path + "successfully");
		std::cout << "Project directory is " << scene->GetDirectory() << std::endl;
	}
};