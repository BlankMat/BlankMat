#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "rendering/shader.h"
#include "rendering/light.h"
#include "rendering/camera.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/node.h"
#include "interfaces/iEntity.h"

#include "containers/textureContainer.h"
#include "containers/materialContainer.h"
#include "containers/shaderContainer.h"
#include "containers/cameraContainer.h"
#include "containers/lightContainer.h"
#include "containers/entityContainer.h"
#include "containers/meshContainer.h"
#include <unordered_map>

class Window;

class IScene
{
protected:
	std::string mDirectory;
	std::string mName;

	IEntity* mGrid = nullptr;
	IEntity* mTransformHandle = nullptr;
	IEntity* mViewAxisHandle = nullptr;

	int mViewAxisPrevWidth = 0;
	int mViewAxisPrevHeight = 0;
	glm::mat4 mViewAxisProjection = glm::mat4(1.0f);

	std::string mCurShader;
	Shader* mShader;
	Light* mGlobalLight;
	Camera* mMainCamera;
	Material* mDefaultMat;
	Node* mRootNode;
	State* mState;

	std::vector<Mesh*> mMeshList;
	std::unordered_map<std::string, EntityContainer*> mMeshRenderList;
	std::unordered_map<std::string, EntityContainer*> mEntityList;

	std::unordered_map<std::string, Light*> mLightList;
	std::unordered_map<std::string, Camera*> mCameraList;
	std::unordered_map<std::string, Shader*> mShaderList;
	std::unordered_map<std::string, Material*> mMaterialList;
	std::unordered_map<std::string, Texture*> mTextureList;
	std::unordered_map<std::string, IEntity*> mPreRenderList;
	std::unordered_map<std::string, IEntity*> mRenderList;

	// Loads the material of the given config. Must be bottom-level config
	Material* LoadMaterial(Config* config, const std::string& name);

	// Loads the given texture or returns the existing one
	Texture* LoadTexture(const std::string& type, const std::string& path, const std::string& defaultName);

	// Returns the projection matrix of the view axis handle
	const glm::mat4& GetViewAxisProjection(Window* window);
public:
	TextureContainer* mTextures;
	MaterialContainer* mMaterials;
	LightContainer* mLights;
	CameraContainer* mCameras;
	ShaderContainer* mShaders;
	MeshContainer* mMeshes;
	EntityContainer* mEntities;

	// Renders the current scene
	void Draw(Window* window, Shader* shader);
	// Renders the current scene's shadows
	void DrawShadows(Window* window, Shader* shader);

	// Focuses the camera on the currently selected object (if any)
	void FocusCamera();

	// Toggles the camera between perspective and orthographic
	void TogglePerspective();

	// Toggles the rotation mode of the camera
	void ToggleCameraRotationMode();

	// Sets the view axis handle to the given object
	void SetViewAxisHandle(IEntity* viewHandle);

	// Sets the grid object
	void SetGrid(IEntity* grid);
	
	// Sets the transform handle object
	void SetTransformHandle(IEntity* handle);

	// Activates the shader with the given name for the scene
	void UseShader(const std::string& name = "");

	// Creates a shader for the scene with the given name from the source file of the given name
	Shader* CreateShader(const std::string& name, bool loadGeom);

	// Creates a shader for the scene with the given name, loading it from a different source than the name
	Shader* CreateShader(const std::string& name, const std::string& source, bool loadGeom);

	// Creates a shader for the scene with the given name, loading it from a config
	Shader* CreateShader(const std::string& name, Config* config);

	// Returns the scene's camera
	Camera* GetCamera();

	// Returns the scene's light
	Light* GetLight();

	// Returns the scene's name
	std::string GetName();

	// Returns the scene's directory
	std::string GetDirectory();

	// Returns the shader with the given name
	Shader* GetShader(const std::string& name);

	// Returns the material with the given name
	Material* GetMaterial(const std::string& name);

	// Returns the default material
	Material* GetDefaultMat();

	// Returns the material with the given name
	Texture* GetTexture(const std::string& name);

	// Returns the entity with the given name
	IEntity* GetEntity(const std::string& name);

	// Adds the given node to the scene
	void AddNode(Node* node);
	// Adds the given mesh to the scene as child of the given node
	void AddMesh(Mesh* mesh, Node* parent = nullptr);
	// Returns the root node
	Node* GetRootNode();
	// Sets the root node
	void SetRootNode(Node* rootNode);
	
	// Returns a reference to the shader list
	const std::unordered_map<std::string, Shader*>& GetShaderList();

	// Returns a reference to the material list
	const std::unordered_map<std::string, Material*>& GetMaterialList();

	// Returns a reference to the texture list
	const std::unordered_map<std::string, Texture*>& GetTextureList();

	// Returns the current shader
	const std::string GetCurShader();

	// Sets up the scene's camera with the given options
	void SetCamera(ActionStack* actionStack, Config* config);

	// Sets the scene's camera to the given camera
	void SetCamera(Camera* cam);

	// Sets the scene's light to the given light
	void SetLight(Light* light);

	// Sets the scene's name
	void SetName(const std::string& name);

	// Sets the scene's directory
	void SetDirectory(const std::string& dir);

	// Adds an entity to the scene's render list
	IEntity* AddEntity(const std::string& shaderName, IEntity* entity, bool preRender = false);

	// Adds a texture to the scene's texture list
	Texture* AddTexture(const std::string& name, Texture* texture);

	// Adds a material to the scene's material list
	Material* AddMaterial(const std::string& name, Material* material);

	// Sets the default material of the scene from the material list if the material exists.
	Material* SetDefaultMaterial(const std::string& name);

	// Sets the default material of the scene. Note: Does not add it to the material list.
	Material* SetDefaultMaterial(Material* material);

	// Sets the material of the given entity
	void SetEntityMaterial(IEntity* entity, Material* material);

	// Loads the materials from the given config
	void LoadMaterials(Config* config);

	// Returns the projection matrix of the scene's camera
	const glm::mat4& GetProjectionMatrix(float aspect);

	// Returns the view matrix of the scene's camera
	const glm::mat4& GetViewMatrix();

	// Constructs the scene, getting everything ready for manual setting
	IScene(State* state);
	// Destructs scene and cleans up all memory used
	~IScene();
};
