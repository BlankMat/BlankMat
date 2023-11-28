#pragma once
#include <iostream>
#include <fstream>

class IWritable
{
public:
	/// <summary>
	/// Reads this item from the file
	/// </summary>
	/// <param name="file">File to read from</param>
	virtual void Read(std::ifstream& file) = 0;

	/// <summary>
	/// Writes this item to the file
	/// </summary>
	/// <param name="file">File to write to</param>
	virtual void Write(std::ofstream& file) = 0;
};