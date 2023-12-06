#pragma once
#include "glIncludes.h"
#include "blankMatConfig.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include "files/sceneWriter.h"
#include "files/sceneReader.h"
#include <iostream>

/// <summary>
/// Class that handles all scene input and output operations by opening popups to select files 
/// and passing all read/write operations to the appropriate classes.
/// </summary>
class SceneIO
{
private:
	/// <summary>
	/// Global state
	/// </summary>
	State* mState = nullptr;

	/// <summary>
	/// Current scene
	/// </summary>
	Scene* mScene = nullptr;

	/// <summary>
	/// Current GLFW window
	/// </summary>
	GLFWwindow* mCurWindow = nullptr;

	/// <summary>
	/// Directory of the current scene
	/// </summary>
	std::string mCurDirectory = "";

	/// <summary>
	/// Filename of the current scene
	/// </summary>
	std::string mCurFileName = "";

	/// <summary>
	/// Extension of the current scene
	/// </summary>
	std::string mCurExtension = "";

	/// <summary>
	/// 
	/// </summary>
	/// <param name="path"></param>
	void ReadPath(const std::string path)
	{
		SplitPath(path, mCurDirectory, mCurFileName, mCurExtension);
		if (mCurExtension != ".blank")
		{
			mCurFileName += mCurExtension;
			mCurExtension = ".blank";
		}
		mState->SetCurFileName(mCurFileName);
	}
public:
	/// <summary>
	/// Returns the full path of the currently open savefile
	/// </summary>
	/// <returns></returns>
	const std::string GetFullPath() const
	{
		return mCurDirectory + mCurFileName + mCurExtension;
	}

	/// <summary>
	/// Saves the scene to a .blank file.
	/// </summary>
	void SaveScene()
	{
		std::cout << "Ran function SaveScene" << std::endl;

		// If no file has been opened, treat this as save scene as
		if (mCurFileName == "")
		{
			SaveSceneAs();
			return;
		}
		std::cout << "Saving scene to file " << mCurFileName << std::endl;
		SceneWriter::SaveScene(mScene, GetFullPath());
		mState->SaveActionStack();
	}

	/// <summary>
	/// Saves the scene to a .blank file of the chosen name.
	/// </summary>
	void SaveSceneAs()
	{
		std::cout << "Ran function SaveSceneAs" << std::endl;
		std::string fileName = pfd::save_file("Save Scene As...", mCurDirectory, { 
			"BlankMat Scenes (.blank)", "*.blank" 
		}, pfd::opt::none).result();

		if (fileName != "")
		{
			ReadPath(fileName);
			std::cout << "Saving scene to file " << GetFullPath() << std::endl;
			SceneWriter::SaveScene(mScene, GetFullPath());
			mState->SaveActionStack();
		}
	}

	/// <summary>
	/// Saves the scene to a .blank file, incrementing the name of the previously saved scene.
	/// </summary>
	void SaveSceneIncrement()
	{
		std::cout << "Ran function SaveSceneIncrement" << std::endl;
		// If no file has been opened, treat this as save scene as
		if (mCurFileName == "")
		{
			SaveSceneAs();
			return;
		}

		// Update filename
		mCurFileName = IncrementName(mCurFileName, 3);
		mState->SetCurFileName(mCurFileName);

		// Save scene
		std::cout << "Saving scene to file " << mCurFileName << std::endl;
		SceneWriter::SaveScene(mScene, GetFullPath());
		mState->SaveActionStack();
	}

	/// <summary>
	/// Opens the chosen .blank scene file.
	/// </summary>
	void OpenScene()
	{
		std::cout << "Ran function OpenScene" << std::endl;
		std::vector<std::string> selection = pfd::open_file("Open Scene", mCurDirectory, { 
			"BlankMat Scenes (.blank)", "*.blank" 
		}, pfd::opt::none).result();

		// If a valid file was selected, prompt the user to save unsaved changes before continuing
		if (!selection.empty() && PromptSaveChanges(mCurWindow, mState))
		{
			ReadPath(selection[0]);
			SceneReader::ReadScene(mScene, GetFullPath(), true);
			mState->SaveActionStack();
		}
	}

	/// <summary>
	/// Creates a new empty .blank scene.
	/// </summary>
	void NewScene()
	{
		std::cout << "Ran function NewScene" << std::endl;
		std::string fileName = pfd::save_file("New Scene...", mCurDirectory, {
			"BlankMat Scenes (.blank)", "*.blank"
			}, pfd::opt::none).result();

		if (fileName != "" && PromptSaveChanges(mCurWindow, mState))
		{
			ReadPath(fileName);
			std::cout << "Saving scene to file " << GetFullPath() << std::endl;
			SceneReader::ClearScene(mScene);
			SceneReader::LoadDefaultScene(mScene);
			SceneWriter::SaveScene(mScene, GetFullPath());
			mState->ClearActionStack();
		}
	}

	/// <summary>
	/// Imports any 3D scene or model, including .blank.
	/// </summary>
	void Import()
	{
		std::cout << "Ran function Import" << std::endl;
		std::vector<std::string> selection = pfd::open_file("Import File", mCurDirectory, {
			"3D Models (.obj)", "*.obj", 
			"BlankMat Scenes (.blank)", "*.blank"
		}, pfd::opt::none).result();

		if (!selection.empty())
		{
			SceneReader::ReadScene(mScene, selection[0], false);
			mState->SaveActionStack();
		}
	}

	/// <summary>
	/// Exports the selected node to any 3D scene, including .blank.
	/// </summary>
	void Export()
	{
		std::cout << "Ran function Export" << std::endl;
		std::string fileName = pfd::save_file("Export", mCurDirectory, {
			"Obj (.obj)", "*.obj",
			"BlankMat Scenes (.blank)", "*.blank"
		}, pfd::opt::none).result();

		if (fileName != "")
		{
			std::string tempDir = "";
			std::string tempFile = "";
			std::string tempExt = "";
			SplitPath(fileName, tempDir, tempFile, tempExt);
			// TODO: Validate export file extension
			SceneWriter::SaveScene(mScene, tempDir + tempFile + tempExt);
		}
	}


	/// <summary>
	/// Prompts to save changes if there are any unsaved changes.
	/// </summary>
	/// <param name="window">The current window of the app</param>
	/// <param name="state">Global state of the app</param>
	/// <returns>Whether to continue with the prompting operation</returns>
	bool PromptSaveChanges(GLFWwindow* window, State* state)
	{
		// If state is saved, break
		if (state->IsSaved())
			return true;

		// If state is not saved, set a popup
		pfd::button res = pfd::message(
			"Unsaved Changes",
			"You have made unsaved changes to the scene. Would you like to save them before opening a new scene?",
			pfd::choice::yes_no_cancel,
			pfd::icon::warning
		).result();

		switch (res)
		{
		case pfd::button::yes:
			// If the user requested to save first, run the save command
			SaveScene();
			return true;
			break;
		case pfd::button::no:
			// If the user did not request to save, don't run the save command
			return true;
			break;
		case pfd::button::cancel:
		default:
			// If the user canceled, do not continue
			return false;
		}
	}

	/// <summary>
	/// Creates a new scene reader/writer
	/// </summary>
	/// <param name="state">Global state of the application</param>
	/// <param name="scene">Current scene</param>
	/// <param name="window">Current window</param>
	explicit SceneIO(State* state, Scene* scene, GLFWwindow* window)
		: mState(state), mScene(scene), mCurWindow(window)
	{
		// No need to initialize anything beyond member variables
	}
};