#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include "files/sceneReader.h"
#include "files/sceneWriter.h"
#include <iostream>

class SceneIO
{
private:
	State* mState = nullptr;
	Scene* mScene = nullptr;
	GLFWwindow* mCurWindow = nullptr;

	std::string mCurDirectory = "";
	std::string mCurFileName = "";
	std::string mCurExtension = "";

	static std::string GetDirectory(const std::string path)
	{
		// Don't parse empty paths
		if (path == "")
			return "";

		size_t lastSlash = path.find_last_of('/') + 1;
		return path.substr(0, lastSlash);
	}

	static std::string GetFileName(const std::string path)
	{
		// Don't parse empty paths
		if (path == "")
			return "";

		size_t lastSlash = path.find_last_of('/') + 1;
		size_t lastPeriod = path.find_last_of('.');
		return path.substr(lastSlash, lastPeriod - lastSlash);
	}

	static std::string GetFileExtension(const std::string path)
	{
		// Don't parse empty paths
		if (path == "")
			return "";

		size_t lastPeriod = path.find_last_of('.');
		if (lastPeriod < path.length())
			return path.substr(lastPeriod - 1);
		else
			return "";
	}

	static void SplitPath(const std::string path, std::string& dir, std::string& name, std::string& ext)
	{
		// Don't parse empty paths
		if (path == "")
			return;

		size_t lastSlash = path.find_last_of('/') + 1;
		size_t lastPeriod = path.find_last_of('.');

		dir = path.substr(0, lastSlash);
		name = path.substr(lastSlash, lastPeriod - lastSlash);

		if (lastPeriod < path.length())
			ext = path.substr(lastPeriod);
		else
			ext = "";
	}

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
	inline const std::string GetFullPath() const
	{
		return mCurDirectory + mCurFileName + mCurExtension;
	}

	void SaveScene()
	{
		std::cout << "Ran command SaveScene" << std::endl;

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

	void SaveSceneAs()
	{
		std::cout << "Ran command SaveSceneAs" << std::endl;
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

	void SaveSceneIncrement()
	{
		std::cout << "Ran command SaveSceneIncrement" << std::endl;
		// TODO: Implement save scene increment
		mState->SaveActionStack();
	}

	void OpenScene()
	{
		std::cout << "Ran command OpenScene" << std::endl;
		auto selection = pfd::open_file("Open Scene", mCurDirectory, { 
			"BlankMat Scenes (.blank)", "*.blank" 
		}, pfd::opt::none).result();

		if (!selection.empty())
		{
			ReadPath(selection[0]);
			SceneReader::LoadScene(mScene, GetFullPath());
			mState->SaveActionStack();
		}
	}

	void NewScene()
	{
		std::cout << "Ran command NewScene" << std::endl;
		// TODO: Implement new scene
	}

	void Import()
	{
		std::cout << "Ran command Import" << std::endl;
		auto selection = pfd::open_file("Import File", mCurDirectory, { 
			"3D Models (.obj)", "*.obj", 
			"BlankMat Scenes (.blank)", "*.blank"
		}, pfd::opt::none).result();

		if (!selection.empty())
		{
			std::string tempDir = "";
			std::string tempFile = "";
			std::string tempExt = "";
			SplitPath(selection[0], tempDir, tempFile, tempExt);
			// TODO: Implement import
		}
	}

	void Export()
	{
		std::cout << "Ran command Export" << std::endl;
		// TODO: Implement export
	}

	explicit SceneIO(State* state, Scene* scene, GLFWwindow* window)
		: mState(state), mScene(scene), mCurWindow(window)
	{
		// No need to initialize anything beyond member variables
	}
};