#include "scene.h"

void Scene::Draw(Window* window)
{
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();
	if (mCurShader == "")
		UseShader(DEFAULT_SHADER);

	// Draw pre-renderables
	for (auto iter = mPreRenderList.begin(); iter != mPreRenderList.end(); ++iter)
		if (iter->second != nullptr)
			iter->second->Draw(viewProj);

	// Draw all of the scene
	UseShader(mCurShader);
	mRootNode->Draw(viewProj);

	// Draw lights
	if (mGlobalLight != nullptr)
		mGlobalLight->Draw(viewProj);

	// Draw post-renderables
	for (auto iter = mRenderList.begin(); iter != mRenderList.end(); ++iter)
	{
		// Render camera axis handle with inverse view matrix
		if (iter->first == CAMERA_AXIS_HANDLE) {
			iter->second->SetRot(mMainCamera->GetRotationDegrees());
			iter->second->Draw(glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -100.0f, 100.0f));
		}
		else if (iter->second != nullptr) {
			iter->second->Draw(viewProj);
		}
	}
}

// Sets the shader for all the meshes of the model
void Scene::SetMeshShaders(Shader* shader, State* state)
{
	for (unsigned int i = 0; i < mMeshList.size(); i++)
	{
		mMeshList[i]->SetShader(shader);
		mMeshList[i]->SetState(state);
	}
}

// Loads the model at the given path
void Scene::LoadModel(std::string path)
{
	std::cout << "Reading model from file " << path << std::endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	mDirectory = path.substr(0, path.find_last_of('/'));

	// Process model
	if (mRootNode == nullptr)
		mRootNode = new Node(nullptr, "root");
	ProcessNode(mRootNode, scene->mRootNode, scene);
	std::cout << "Read model from file " << path << " successfully, new directory is " << mDirectory << std::endl;
}

// Recursively processes the meshes in the given node and all its children
void Scene::ProcessNode(Node* sceneNode, aiNode* node, const aiScene* scene)
{
	std::cout << " - Reading node " << node->mName.C_Str() << std::endl;
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* newMesh = ProcessMesh(mesh, scene);
		newMesh->SetDefaultMat(GetMaterial("default"));
		sceneNode->AddMesh(newMesh);
		mMeshList.push_back(newMesh);
	}
	// Process each of the children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		Node* childNode = new Node(sceneNode, node->mChildren[i]->mName.C_Str());
		sceneNode->AddChild(childNode);
		ProcessNode(childNode, node->mChildren[i], scene);
	}
}

// Process the vertices, indices, and textures of the given mesh
Mesh* Scene::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material* newMaterial;

	// Process each vertex
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 pos = Vec3FromAssimp(mesh->mVertices[i]);
		glm::vec3 normal = Vec3FromAssimp(mesh->mNormals[i]);
		glm::vec2 texCoords = (mesh->mTextureCoords[0]) ? Vec2FromAssimp(mesh->mTextureCoords[0][i]) : glm::vec2();
		glm::vec3 tangent = Vec3FromAssimp(mesh->mTangents[i]);
		vertices.push_back(Vertex(pos, normal, texCoords, tangent));
	}

	// Process each index
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);

	// Process each material's diffuse and specular maps
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		newMaterial = GetMaterial(material->GetName().C_Str());
		if (newMaterial == nullptr)
		{
			// 1. diffuse maps
			std::vector<Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			if (diffuseMaps.size() == 0)
				diffuseMaps.push_back(GetTexture("default_diffuse"));
			// 2. ambient maps
			std::vector<Texture*> ambientMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
			if (ambientMaps.size() == 0)
				ambientMaps.push_back(GetTexture("default_ambient"));
			// 3. specular maps
			std::vector<Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			if (specularMaps.size() == 0)
				specularMaps.push_back(GetTexture("default_specular"));
			// 4. normal maps
			std::vector<Texture*> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			if (normalMaps.size() == 0)
				normalMaps.push_back(GetTexture("default_normal"));
			// 5. height maps
			std::vector<Texture*> heightMaps = LoadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_height");
			if (heightMaps.size() == 0)
				heightMaps.push_back(GetTexture("default_height"));
			// 6. alpha maps
			std::vector<Texture*> alphaMaps = LoadMaterialTextures(material, aiTextureType_OPACITY, "texture_alpha");
			if (alphaMaps.size() == 0)
				alphaMaps.push_back(GetTexture("default_alpha"));

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
			newMaterial = new Material(diffuseMaps, ambientMaps, specularMaps, normalMaps, heightMaps, alphaMaps,
				ka, kd, ks, ns, ni, d, ke);
			AddMaterial(material->GetName().C_Str(), newMaterial);
		}
	}
	// If there are no textures, use default material
	else
	{
		newMaterial = GetMaterial("default");
	}

	return new Mesh(vertices, indices, newMaterial);
}

// Load
std::vector<Texture*> Scene::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture is loaded already
		Texture* texture = GetTexture(str.C_Str());
		if (texture != nullptr)
		{
			textures.push_back(texture);
			continue;
		}

		// if texture hasn't been loaded already, load it
		texture = new Texture(typeName, this->mDirectory, str.C_Str());
		textures.push_back(texture);
		mTextureList.emplace(str.C_Str(), texture);
		std::cout << "  - Loaded texture " << str.C_Str() << std::endl;
	}
	return textures;
}

// Constructs the scene from the given file
Scene::Scene()
{
	mCurShader = "";
	mMainCamera = nullptr;
	mGlobalLight = nullptr;
	mRootNode = nullptr;
	mRenderList = std::unordered_map<std::string, IEntity*>();
	mPreRenderList = std::unordered_map<std::string, IEntity*>();
	mShaderList = std::unordered_map<std::string, Shader*>();
	mMaterialList = std::unordered_map<std::string, Material*>();
	mTextureList = std::unordered_map<std::string, Texture*>();
	mMeshList = std::vector<Mesh*>();
}

Scene::~Scene()
{
	if (mMainCamera != nullptr)
		delete mMainCamera;
	if (mGlobalLight != nullptr)
		delete mGlobalLight;

	for (auto iter = mShaderList.begin(); iter != mShaderList.end(); ++iter)
	{
		if (iter->second != nullptr)
			delete iter->second;
	}
	for (auto iter = mRenderList.begin(); iter != mRenderList.end(); ++iter)
	{
		if (iter->second != nullptr) {
			iter->second->Cleanup();
			delete iter->second;
		}
	}
	for (auto iter = mPreRenderList.begin(); iter != mPreRenderList.end(); ++iter)
	{
		if (iter->second != nullptr) {
			iter->second->Cleanup();
			delete iter->second;
		}
	}
	for (auto iter = mMaterialList.begin(); iter != mMaterialList.end(); ++iter)
		if (iter->second != nullptr)
			delete iter->second;

	for (auto iter = mTextureList.begin(); iter != mTextureList.end(); ++iter)
		if (iter->second != nullptr)
			delete iter->second;

	for (unsigned int i = 0; i < mMeshList.size(); i++)
		if (mMeshList[i] != nullptr)
			delete mMeshList[i];

	mShaderList.clear();
	mPreRenderList.clear();
	mRenderList.clear();
	mMaterialList.clear();
	mTextureList.clear();
	mMeshList.clear();
}