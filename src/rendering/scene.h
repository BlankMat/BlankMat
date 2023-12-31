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
#include "primitives/pGrid.h"
#include "primitives/pHandle.h"

#include "containers/textureContainer.h"
#include "containers/materialContainer.h"
#include "containers/shaderContainer.h"
#include "containers/cameraContainer.h"
#include "containers/lightContainer.h"
#include "containers/entityContainer.h"
#include "containers/meshContainer.h"
#include <unordered_map>
#include <unordered_set>

enum class LightingMode
{
	UNLIT = 0,
	FLAT,
	LIT,
	TEXTURED,
	WIREFRAME
};

class Window;

class Scene : public INameable
{
protected:
	std::string mDirectory = "";

	IEntity* mGrid = nullptr;
	IEntity* mTransformHandle = nullptr;
	IEntity* mViewAxisHandle = nullptr;

	int mViewAxisPrevWidth = 0;
	int mViewAxisPrevHeight = 0;

	glm::mat4 mViewAxisProjection = glm::mat4(1.0f);

	std::string mCurShader = "";
	Shader* mShader = nullptr;
	Node* mRootNode = nullptr;
	State* mState = nullptr;
	LightingMode mMode = LightingMode::FLAT;
	static const inline float WIREFRAME_LINE = 0.5f;

	std::unordered_map<std::string, EntityContainer*> mMeshRenderList;
	std::unordered_set<std::string> mScopes;
	
	TextureContainer* mTextures = nullptr;
	MaterialContainer* mMaterials = nullptr;
	LightContainer* mLights = nullptr;
	CameraContainer* mCameras = nullptr;
	ShaderContainer* mShaders = nullptr;
	MeshContainer* mMeshes = nullptr;
	EntityContainer* mEntities = nullptr;

	// Loads the material of the given config. Must be bottom-level config
	Material* LoadMaterial(Config* config, const std::string& name, const std::string& scope);

	// Loads the given texture or returns the existing one
	Texture* LoadTexture(TextureType type, const std::string& path, const std::string& scope, const std::string& defaultName);

	// Enables or disables wireframe
	void HandleWireframe();

	// Returns the projection matrix of the view axis handle
	const glm::mat4& GetViewAxisProjection(Window* window);

	// Recursively deletes the contents of the node (children and meshes)
	void DeleteNodeContents(Node* node);
public:
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

	// Activates the shader with the given name for the scene
	void UseShader(const std::string& name = "");

	// Returns the texture container of the scene
	TextureContainer* GetTextures();

	// Returns the material container of the scene
	MaterialContainer* GetMaterials();

	// Returns the light container of the scene
	LightContainer* GetLights();

	// Returns the camera container of the scene
	CameraContainer* GetCameras();

	// Returns the shader container of the scene
	ShaderContainer* GetShaders();

	// Returns the mesh container of the scene
	MeshContainer* GetMeshes();

	// Returns the entity container of the scene
	EntityContainer* GetEntities();

	// Returns the scene's camera
	Camera* GetCamera();

	// Returns the scene's light
	Light* GetLight();

	// Returns the scene's name
	std::string GetName();

	// Returns the scene's directory
	std::string GetDirectory();

	// Returns the current shader
	const std::string GetCurShader();

	// Returns the root node
	Node* GetRootNode();

	// Returns the shader with the given name
	Shader* GetShader(const std::string& name);

	// Returns the material with the given name
	Material* GetMaterial(const std::string& name);

	// Returns the default material
	Material* GetDefaultMaterial();

	// Returns the material with the given name
	Texture* GetTexture(const std::string& name);

	// Returns the transform handle
	IEntity* GetTransformHandle();

	// Returns the state of the application
	State* GetState();

	// Returns the lighting mode of the application
	LightingMode GetLightingMode();

	// Returns a unique scope, using the given name as the starting point
	std::string GetUniqueScope(const std::string& scope);

	// Returns whether the given scope is active in the scene
	bool IsScopeActive(const std::string& scope);

	// Adds the given node to the scene
	void AddNode(Node* node);

	// Adds the given mesh to the scene as child of the given node
	Mesh* AddMesh(Mesh* mesh, Node* parent = nullptr);

	// Adds the given camera to the scene
	Camera* AddCamera(const std::string& name, Camera* camera, bool select = false, bool replace = false);

	// Adds the given light to the scene
	Light* AddLight(const std::string& name, Light* light, bool select = false, bool replace = false);

	// Adds a texture to the scene's texture list
	Texture* AddTexture(const std::string& name, Texture* texture, bool replace = false);

	// Adds a material to the scene's material list
	Material* AddMaterial(const std::string& name, Material* material, bool replace = false);

	// Creates a shader for the scene with the given name from the source file of the given name
	Shader* CreateShader(const std::string& name, bool loadGeom);

	// Creates a shader for the scene with the given name, loading it from a different source than the name
	Shader* CreateShader(const std::string& name, const std::string& source, bool loadGeom);

	// Creates a shader for the scene with the given name, loading it from a config
	Shader* CreateShader(const std::string& name, Config* config);

	// Deletes the given mesh
	void DeleteMesh(Mesh* mesh);

	// Deletes the given node
	void DeleteNode(Node* node);

	// Delete the given item
	void DeleteSelectable(ISelectable* item);

	// Deletes the current selection
	void DeleteSelection();

	// Updates the scene's material render list
	void UpdateRenderList();
	
	// Rotates the current camera by the given delta degrees
	void RotateCamera(const glm::vec3& delta);

	// Translates the current camera by the given delta
	void TranslateCamera(const glm::vec3& delta);

	// Sets the root node
	void SetRootNode(Node* rootNode);

	// Sets the scene's camera to the given camera
	void SetCamera(const std::string& camera);

	// Sets the scene's light to the given light
	void SetLight(const std::string& light);

	// Sets the scene's name
	void SetName(const std::string& name);

	// Sets the scene's directory
	void SetDirectory(const std::string& dir);

	// Sets the lighting mode of the scene
	void SetLightingMode(LightingMode mode);

	// Sets the material of the given node
	void SetNodeMaterial(Node* node, Material* material);

	// Sets the material of the given entity
	void SetEntityMaterial(IEntity* entity, Material* material);

	// Loads the materials from the given config
	void LoadMaterials(Config* config);

	// Returns the projection matrix of the scene's camera
	const glm::mat4& GetProjectionMatrix(float aspect);

	// Returns the view matrix of the scene's camera
	const glm::mat4& GetViewMatrix();

	// Clears the scene completely
	void Clear();

	// Clears all items that would render
	void ClearRendering();

	// Constructs the scene, getting everything ready for manual setting
	Scene(State* state);

	// Destructs scene and cleans up all memory used
	~Scene();
};
