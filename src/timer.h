#pragma once
#include "glIncludes.h"
#include <iostream>

class Timer
{
public:	
	static void Time(double startTime, const std::string& operationName)
	{
		double endTime = glfwGetTime();
		double funcTime = endTime - startTime;
		std::cout << operationName << " in " << funcTime << " seconds." << std::endl;
	}
};