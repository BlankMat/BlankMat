#pragma once
#include <iostream>
#include <fstream>

class IWritable
{
public:
	/// <summary>
	/// Returns the number of elements that will be written as part of the object
	/// </summary>
	/// <returns>Number of elements to be written</returns>
	virtual unsigned int WriteCount() = 0;

	/// <summary>
	/// Reads this item from the file
	/// </summary>
	/// <param name="scope">Scope to read in</param>
	/// <param name="file">File to read from</param>
	/// <param name="clear">Whether to overwrite the contents of the item</param>
	virtual void Read(const std::string& scope, std::ifstream& file, bool clear) = 0;

	/// <summary>
	/// Writes this item to the file
	/// </summary>
	/// <param name="file">File to write to</param>
	virtual void Write(std::ofstream& file) = 0;

	/// <summary>
	/// Clears this object
	/// </summary>
	virtual void Clear() = 0;
};