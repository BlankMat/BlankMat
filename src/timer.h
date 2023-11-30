#pragma once
#include "glIncludes.h"
#include <iostream>

class Timer
{
private:
	int mNumFrames = 0;
	double mPrevSecondTime = 0.0;
	double mPrevFrameTime = 0.0;
	double mCurFrameTime = 0.0;
	float mDeltaTime = 0.0f;
	std::string mFPS = "";
	std::string mFrameTime = "";

public:
	static double Start()
	{
		return glfwGetTime();
	}

	static void Time(double startTime, const std::string& operationName)
	{
		double endTime = glfwGetTime();
		double funcTime = endTime - startTime;
		std::cout << operationName << " in " << funcTime << " seconds." << std::endl;
	}

	float GetDeltaTime()
	{
		return mDeltaTime;
	}

	std::string GetFPS()
	{
		return mFPS;
	}

	std::string GetFrameTime()
	{
		return mFrameTime;
	}

	void HandleFrame()
	{
		// Calculate delta time
		mPrevFrameTime = mCurFrameTime;
		mCurFrameTime = glfwGetTime();
		mDeltaTime = float(mCurFrameTime - mPrevFrameTime);
		mNumFrames++;

		// Check whether a full second has passed since the last update
		if (mCurFrameTime - mPrevSecondTime >= 1.0)
		{
			// Calculate FPS and Frame Length in ms
			mFPS = std::to_string(mNumFrames);
			std::string frameTime = std::to_string(1000.0f / mNumFrames);
			mFrameTime = frameTime.substr(0, frameTime.find(".") + 3) + "ms";

			// Reset frame times
			mNumFrames = 0;
			mPrevSecondTime = mCurFrameTime;
		}
	}
};