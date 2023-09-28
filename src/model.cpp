#include "model.h"
#include "mathLib.h"

Model::Model(std::string path)
{
	LoadModel(path);
}

// Draws each mesh in the model onto the screen
void Model::Draw(glm::mat4 viewProj)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(viewProj);
}

// Sets the shader for all the meshes of the model
void Model::SetMeshShaders(Shader* shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].SetShader(shader);
}

// Loads the model at the given path
void Model::LoadModel(std::string path)
{
	std::cout << "Reading model from file " << path << std::endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	// Load default texture regardless of whether it's used or not
	LoadDefaultTexture();

	// Process model
	ProcessNode(scene->mRootNode, scene);
	std::cout << "Read model from file " << path << " successfully, new directory is " << directory << std::endl;
}

// Recursively processes the meshes in the given node and all its children
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	std::cout << " - Reading node " << node->mName.C_Str() << std::endl;
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	// Process each of the children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

// Process the vertices, indices, and textures of the given mesh
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Process each vertex
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 pos = Vec3FromAssimp(mesh->mVertices[i]);
		glm::vec3 normal = Vec3FromAssimp(mesh->mNormals[i]);
		glm::vec2 texCoords = (mesh->mTextureCoords[0]) ? Vec2FromAssimp(mesh->mTextureCoords[0][i]) : glm::vec2();
		vertices.push_back(Vertex(pos, normal, texCoords));
	}

	// Process each index
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);

	// Process each material's diffuse and specular maps
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	// If the model does not have a texture, use the default texture
	if (textures.size() == 0)
	{
		LoadDefaultTexture();
		textures.push_back(texturesLoaded[defaultTextureIndex]);
		std::cout << "  - Mesh has no texture, using default texture" << std::endl;
	}
	return Mesh(vertices, indices, textures);
}

// Loads the default texture
void Model::LoadDefaultTexture()
{
	// If the default texture is loaded already, exit
	if (defaultTextureIndex >= 0 && defaultTextureIndex < texturesLoaded.size())
		return;

	std::string defaultPath = "default.png";
	unsigned int textureID = TextureFromFile(defaultPath.c_str(), FileSystem::GetPath("resources/textures"));
	std::cout << "  - Loaded default texture from " << FileSystem::GetPath("resources/textures/") << defaultPath << std::endl;
	defaultTextureIndex = (int)texturesLoaded.size();
	texturesLoaded.push_back(Texture(textureID, "texture_diffuse", defaultPath));
}

// Load
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture is loaded already
		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			// If the texture is already loaded, add it to the mesh
			if (std::strcmp(texturesLoaded[j].path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		// Skip to the next texture if already loaded
		if (!skip)
		{
			// if texture hasn't been loaded already, load it
			unsigned int textureId = TextureFromFile(str.C_Str(), this->directory);
			Texture texture = Texture(textureId, typeName, str.C_Str());
			textures.push_back(texture);
			// store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			texturesLoaded.push_back(texture);
			std::cout << "  - Loaded texture " << str.C_Str() << std::endl;
		}
	}
	return textures;
}

// Loads the given texture from a file
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}