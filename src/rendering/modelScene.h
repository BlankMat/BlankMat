#pragma once
#include "glIncludes.h"
#include "iScene.h"
#include "model.h"

class ModelScene : public IScene
{
protected:
	Model* mCurModel;
public:
	// Renders the current scene
	void Draw(Window* window) override;

	// Returns the scene's model
	Model* GetModel() { return mCurModel; }

	// Sets the scene's model to the given model
	void SetModel(Model* model) { if (mCurModel != nullptr) { delete mCurModel; } mCurModel = model; }

	// Constructs the scene, getting everything ready for manual setting
	ModelScene();

	// Destructs scene and cleans up all memory used
	~ModelScene();
};
