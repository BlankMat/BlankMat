#pragma once
#include "glIncludes.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

/// <summary>
/// Container class for storing key value pairs. 
/// Should not be instantiated directly, but should be overridden by a class that implements additional functionality.
/// (If not overriden, this class is functionally equivalent to an unordered map, just use that instead)
/// </summary>
/// <typeparam name="T">Type of item to store</typeparam>
template<typename T>
class IContainer
{
protected:
	/// <summary>
	/// Data of the container
	/// </summary>
	std::unordered_map<std::string, T*> mData;

	/// <summary>
	/// Currently selected item's name
	/// </summary>
	std::string mCurSelectedName;

	/// <summary>
	/// Currently selected item
	/// </summary>
	T* mCurSelectedItem;

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	virtual const std::pair<std::string, T*> ReadItem(std::ifstream& file) = 0;

	/// <summary>
	/// Writes the given item into the output file stream
	/// </summary>
	/// <param name="key">Key of item</param>
	/// <param name="item">Item to write</param>
	/// <param name="file">File to write to</param>
	virtual void WriteItem(const std::string& key, T* item, std::ofstream& file) = 0;
public:
	/// <summary>
	/// Reads this container from the file
	/// </summary>
	/// <param name="file">File to read from</param>
	void Read(std::ifstream& file)
	{
		std::string line;
		while (std::getline(file, line))
		{
			// Don't parse empty lines
			if (line == "")
				continue;

			std::vector<std::string> parse;
			ParseStringByDelim(parse, line, " ");

			// Don't parse empty lines
			if (parse.empty())
				continue;

			// Check for tag markers
			if (parse[0] == "##StartItem")
			{
				auto newItem = ReadItem(file);
				if (newItem->first != nullptr && newItem->second != nullptr)
					Add(newItem->first, newItem->second);
			}
			// End parsing if the tag is found
			else if (parse[0] == "#EndContainer")
				break;
		}
	}

	/// <summary>
	/// Writs this container to the file
	/// </summary>
	/// <param name="file">File to output to</param>
	void Write(std::ofstream& file)
	{
		file << "#StartContainer" << std::endl;
		file << "#NumItems " << std::to_string(mData.size()) << std::endl;
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			file << "##StartItem" << std::endl;
			WriteItem(iter->first, iter->second, file);
			file << "##EndItem" << std::endl;
			file << std::endl;
		}
		file << "#EndContainer" << std::endl;
		file << std::endl;
	}

	/// <summary>
	/// Returns the number of elements in the container
	/// </summary>
	/// <returns></returns>
	unsigned int Count()
	{
		return (unsigned int)mData.size();
	}

	/// <summary>
	/// Returns whether the container is empty or not
	/// </summary>
	/// <returns></returns>
	bool Empty()
	{
		return mData.size() == 0;
	}

	/// <summary>
	/// Selects the given item. Does nothing if the item is not part of the container.
	/// </summary>
	/// <param name="name">Name of item to select</param>
	/// <returns>Whether the item was selected or not</returns>
	virtual bool Select(const std::string& name)
	{
		T* item = GetItem(name);
		if (item != nullptr)
		{
			mCurSelectedItem = item;
			mCurSelectedName = name;
			return true;
		}
		return false;
	}

	/// <summary>
	/// Selects the given item. Does nothing if the item is not part of the container.
	/// </summary>
	/// <param name="item">Item to select</param>
	/// <returns>Whether the item was selected or not</returns>
	virtual bool Select(T* item)
	{
		std::string name = GetKey(item);
		if (name != "")
		{
			mCurSelectedItem = item;
			mCurSelectedName = name;
			return true;
		}
		return false;
	}

	/// <summary>
	/// Deselects the currently selected item
	/// </summary>
	virtual void Deselect()
	{
		mCurSelectedItem = nullptr;
		mCurSelectedName = "";
	}

	/// <summary>
	/// Returns the currently selected item
	/// </summary>
	/// <returns>Selected item</returns>
	virtual const std::string GetSelectedName()
	{
		return mCurSelectedName;
	}

	/// <summary>
	/// Returns the currently selected item
	/// </summary>
	/// <returns>Selected item</returns>
	virtual T* GetSelectedItem()
	{
		return mCurSelectedItem;
	}

	/// <summary>
	/// Adds the given item to the container, or if it already exists, updates the item to the given one
	/// </summary>
	/// <param name="name">Name of the item</param>
	/// <param name="item">The item to store</param>
	virtual void Add(const std::string& name, T* item)
	{
		if (mData.find(name) == mData.end())
			mData.emplace(name, item);
		else
			mData[name] = item;
	}

	/// <summary>
	/// Removes the given item from the container
	/// </summary>
	/// <param name="name">Name of item to remove</param>
	/// <returns>Whether the item was removed or not</returns>
	virtual bool Remove(const std::string& name)
	{
		if (mData.find(name) != mData.end())
		{
			mData.erase(name);
			return true;
		}
		return false;
	}

	/// <summary>
	/// Removes the given item from the container
	/// </summary>
	/// <param name="item">Item to remove</param>
	/// <returns>Whether the item was removed or not</returns>
	virtual bool Remove(T* item)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			if (iter->second == item)
			{
				mData.erase(iter->first);
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Returns the item with the given key, or nullptr if not found
	/// </summary>
	/// <param name="name">Name of item to find</param>
	/// <returns>Item</returns>
	virtual T* GetItem(const std::string& name)
	{
		if (mData.find(name) != mData.end())
			return mData[name];
		return nullptr;
	}

	/// <summary>
	/// Returns the key of the item given, or an empty string if not found
	/// </summary>
	/// <param name="item">Item to search for</param>
	/// <returns>Name of item</returns>
	virtual const std::string GetKey(T* item)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			if (iter->second == item)
			{
				return iter->first;
			}
		}
		return "";
	}

	/// <summary>
	/// Clears the container of all items, deleting them
	/// </summary>
	void Clear()
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			delete iter->second;
		}
		mData.clear();
	}

	/// <summary>
	/// Returns a reference to the data stored in the container.
	/// WARNING: Only use this to iterate over the data, not to modify it
	/// </summary>
	/// <returns>Data of the container</returns>
	virtual const std::unordered_map<std::string, T*>& Data()
	{
		return mData;
	}
};