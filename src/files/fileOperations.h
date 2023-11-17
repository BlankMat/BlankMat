#pragma once
#include "glIncludes.h"
#include "tools/state.h"
#include "rendering/scene.h"
#include <iostream>

class FileOperations
{
private:
	State* mState;
	Scene* mScene;

	std::string mCurDirectory;
public:
	void SaveScene()
	{
		std::cout << "Ran command SaveScene" << std::endl;
	}

	void SaveSceneAs()
	{
		std::cout << "Ran command SaveSceneAs" << std::endl;
	}

	void SaveSceneIncrement()
	{
		std::cout << "Ran command SaveSceneIncrement" << std::endl;
	}

	void OpenScene()
	{
		std::cout << "Ran command OpenScene" << std::endl;
		auto selection = pfd::open_file("Open Scene", mCurDirectory, { "BlankMat Scenes", "*.bm" }, pfd::opt::none).result();
		if (!selection.empty())
		{
			std::string fileName = selection[0];
			mScene->LoadModel(fileName);
		}
	}

	void NewScene()
	{
		std::cout << "Ran command NewScene" << std::endl;
	}

	void Import()
	{
		std::cout << "Ran command Import" << std::endl;
		auto selection = pfd::open_file("Import File", mCurDirectory, { "3D Models", "*.obj", "BlankMat Scenes", "*.bm"}, pfd::opt::none).result();
		if (!selection.empty())
		{
			std::string fileName = selection[0];
		}
	}

	void Export()
	{
		std::cout << "Ran command Export" << std::endl;
	}

	void SetCurDirectory(const std::string& dir)
	{
		mCurDirectory = dir;
	}

	explicit FileOperations(State* state, Scene* scene)
		: mState(state), mScene(scene)
	{

	}
};