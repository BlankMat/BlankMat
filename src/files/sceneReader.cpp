#include "files/sceneReader.h"
#include "timer.h"
#include "primitives/pLightCube.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Recursively processes the meshes in the given node and all its children
void SceneReader::ProcessAssimpNode(const std::string& scope, Scene* scene, Node* node, const aiNode* assimpNode, const aiScene* assimpScene)
{
	std::cout << " - Reading node " << assimpNode->mName.C_Str() << std::endl;
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < assimpNode->mNumMeshes; i++)
	{
		aiMesh* mesh = assimpScene->mMeshes[assimpNode->mMeshes[i]];
		Mesh* newMesh = ProcessAssimpMesh(scope, scene, mesh, assimpScene);
		scene->AddMesh(newMesh, node);
	}
	// Process each of the children
	for (unsigned int i = 0; i < assimpNode->mNumChildren; i++)
	{
		Node* childNode = new Node(node, assimpNode->mChildren[i]->mName.C_Str());
		node->AddChild(childNode);
		ProcessAssimpNode(scope, scene, childNode, assimpNode->mChildren[i], assimpScene);
	}
}

// Process the vertices, indices, and textures of the given mesh
Mesh* SceneReader::ProcessAssimpMesh(const std::string& scope, Scene* scene, const aiMesh* assimpMesh, const aiScene* assimpScene)
{
	// Process each part of the mesh
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	ProcessAssimpVertices(assimpMesh, vertices);
	ProcessAssimpIndices(vertices, indices, assimpMesh);
	Material* newMaterial = ProcessAssimpMaterial(scope, scene, assimpMesh, assimpScene);

	// Construct mesh
	return new Mesh(assimpMesh->mName.C_Str(), scope, vertices, indices, newMaterial);
}

// Processes the vertices of the mesh, emplacing them into the list given
void SceneReader::ProcessAssimpVertices(const aiMesh* mesh, std::vector<Vertex>& outVertices)
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
void SceneReader::ProcessAssimpIndices(const std::vector<Vertex>& vertices, std::vector<unsigned int>& outIndices, const aiMesh* assimpMesh)
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
Material* SceneReader::ProcessAssimpMaterial(const std::string& scope, Scene* scene, const aiMesh* assimpMesh, const aiScene* assimpScene)
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
	std::vector<Texture*> diffuseMaps;
	LoadMaterialTextures(scope, scene, diffuseMaps, material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
	if (diffuseMaps.empty())
		diffuseMaps.push_back(scene->GetTexture("default_diffuse"));

	// 2. ambient maps
	std::vector<Texture*> ambientMaps;
	LoadMaterialTextures(scope, scene, ambientMaps, material, aiTextureType_AMBIENT, TextureType::AMBIENT);
	if (ambientMaps.empty())
	{
		if (!diffuseMaps.empty())
		{
			Texture* ambientTexture = new Texture(diffuseMaps[0]->GetUnscopedName(), diffuseMaps[0]->GetScope(), diffuseMaps[0]->mID, TextureType::AMBIENT, diffuseMaps[0]->mDir);
			scene->AddTexture(ambientTexture->GetScopedName(), ambientTexture);
			ambientMaps.push_back(ambientTexture);
		}
		else
		{
			ambientMaps.push_back(scene->GetTexture("default_ambient"));
		}
	}

	// 3. specular maps
	std::vector<Texture*> specularMaps;
	LoadMaterialTextures(scope, scene, specularMaps, material, aiTextureType_SPECULAR, TextureType::SPECULAR);
	if (specularMaps.empty())
		specularMaps.push_back(scene->GetTexture("default_specular"));

	// 4. normal maps
	std::vector<Texture*> normalMaps;
	LoadMaterialTextures(scope, scene, normalMaps, material, aiTextureType_HEIGHT, TextureType::NORMAL);
	if (normalMaps.empty())
		normalMaps.push_back(scene->GetTexture("default_normal"));

	// 5. height maps
	std::vector<Texture*> heightMaps;
	LoadMaterialTextures(scope, scene, heightMaps, material, aiTextureType_DISPLACEMENT, TextureType::HEIGHT);
	if (heightMaps.empty())
		heightMaps.push_back(scene->GetTexture("default_height"));

	// 6. alpha maps
	std::vector<Texture*> alphaMaps;
	LoadMaterialTextures(scope, scene, alphaMaps, material, aiTextureType_OPACITY, TextureType::ALPHA);
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
	newMaterial = new Material(material->GetName().C_Str(), scope, diffuseMaps, ambientMaps, specularMaps, normalMaps, heightMaps, alphaMaps,
		ka, kd, ks, ns, ni, d, ke);
	scene->AddMaterial(material->GetName().C_Str(), newMaterial);
	return newMaterial;
}

// Loads material textures
void SceneReader::LoadMaterialTextures(const std::string& scope, Scene* scene, std::vector<Texture*>& outTextures, const aiMaterial* assimpMaterial, aiTextureType assimpTextureType, TextureType type)
{
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
			outTextures.push_back(texture);
			continue;
		}

		// if texture hasn't been loaded already, load it
		outTextures.push_back(scene->AddTexture(textureName, new Texture(textureName, scope, type, scene->GetDirectory(), path)));
		std::cout << "  - Loaded texture " << textureName << std::endl;
	}
}

void SceneReader::ReadAssimpScene(Scene* scene, const std::string& path, bool replace)
{
	double loadStartTime = Timer::Start();
	std::cout << "Reading model from file " << path << std::endl;
	Assimp::Importer importer;
	const aiScene* assimpScene = importer.ReadFile(path, 
		aiProcess_JoinIdenticalVertices | 
		aiProcess_FindInvalidData |
		aiProcess_Triangulate | 
		aiProcess_FlipUVs | 
		aiProcess_GenNormals | 
		aiProcess_FixInfacingNormals | 
		aiProcess_CalcTangentSpace);

	// If an error was encountered when opening the scene, quit reading
	if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	// Split path into the appropriate parts
	std::string dir;
	std::string file;
	std::string ext;
	SplitPath(path, dir, file, ext);

	// If replacing the previous scene, set this scene to be the scope
	if (replace)
	{
		scene->ClearRendering();
		scene->SetDirectory(dir);
		scene->SetScope(file);
		scene->SetName(file);
	}

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
	ProcessAssimpNode(file, scene, sceneRootNode, assimpScene->mRootNode, assimpScene);
	rootNode->SetParentModelMatrix(glm::mat4(1.0f));

	Timer::Time(loadStartTime, "Read model from file " + path + " successfully");
	std::cout << "Project directory is " << scene->GetDirectory() << ", scope is " << scene->GetScope() << std::endl;
}

void SceneReader::ReadBlankMatItem(IWritable* container, bool replace, std::ifstream& file, const std::string& label)
{
	std::cout << "Reading " << label << std::endl;
	double startTime = Timer::Start();
	container->Read(file, replace);
	Timer::Time(startTime, "Read " + std::to_string(container->WriteCount()) + " " + label);
}

void SceneReader::ReadBlankMatScene(Scene* scene, const std::string& path, bool replace)
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
		// If replacing the scene, clear it
		if (replace)
			ClearScene(scene);

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
		scene->GetLights()->LoadMaterials(scene->GetMaterials(), scene->GetTextures());
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

void SceneReader::ClearScene(Scene* scene)
{
	// Clears the scene of everything
	scene->Clear();
}

void SceneReader::LoadDefaultScene(Scene* scene)
{
	scene->AddCamera("main", new Camera(scene->GetState()->GetActionStack()), true);
	scene->AddLight("global", new PLightCube(1.0f, "global", "", LightType::POINT, scene->GetMaterials(), scene->GetTextures()), true);
}

void SceneReader::ReadScene(Scene* scene, const std::string& path, bool replace)
{
	// Get extension
	std::string ext = GetFileExtension(path);

	// Disable rendering until reading is done
	scene->GetState()->shouldRender = false;
	if (ext == BLANK)
	{
		ReadBlankMatScene(scene, path, replace);
	}
	else
	{
		ReadAssimpScene(scene, path, replace);
	}
	scene->GetState()->shouldRender = true;
}