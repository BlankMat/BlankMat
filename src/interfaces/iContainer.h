#pragma once
#include "glIncludes.h"
#include "utils.h"
#include "interfaces/iWritable.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

/// <summary>
/// Container class for storing key value pairs. 
/// Should not be instantiated directly, but should be overridden by a class that implements additional functionality.
/// (If not overriden, this class is functionally equivalent to an unordered map, just use that instead)
/// </summary>
/// <typeparam name="T">Type of item to store</typeparam>
template<typename T>
class IContainer : public IWritable
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
	/// Returns whether the item should be skipped or not
	/// </summary>
	/// <param name="item">Item to consider</param>
	/// <returns>Whether the item should be skipped</returns>
	virtual bool SkipItem(T* item)
	{
		return false;
	}

	/// <summary>
	/// Renames the given item to the given name
	/// </summary>
	/// <param name="item">Item to rename</param>
	virtual void RenameItem(T* item, const std::string& name) = 0;

	/// <summary>
	/// Reads the next item from the input file stream
	/// </summary>
	/// <param name="scope">Scope to read item in</param>
	/// <param name="file">File to read</param>
	/// <returns>Newly constructed item from file</returns>
	virtual const std::pair<std::string, T*> ReadItem(const std::string& scope, std::ifstream& file) = 0;

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
	/// <param name="scope">Scope to read in</param>
	/// <param name="file">File to read from</param>
	/// <param name="clear">Whether to overwrite the contents of the item</param>
	void Read(const std::string& scope, std::ifstream& file, bool clear) override
	{
		if (clear)
			Clear();

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

			// Look for number of items
			if (parse[0] == "items" && parse.size() > 1)
			{
				int numItems = std::stoi(parse[1]);
				for (int i = 0; i < numItems; i++)
				{
					auto newItem = ReadItem(scope, file);
					if (newItem.first != "" && newItem.second != nullptr)
						Add(Scope(newItem.first, scope), newItem.second, false);
				}

				// Once the correct number of items has been read, the container is over
				break;
			}
		}
	}

	/// <summary>
	/// Writes this container to the file
	/// </summary>
	/// <param name="file">File to output to</param>
	void Write(std::ofstream& file) override
	{
		file << "items " << std::to_string(WriteCount());
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			// Skip internal items
			if (SkipItem(iter->second))
				continue;

			file << std::endl;
			WriteItem(iter->first, iter->second, file);
		}
	}

	/// <summary>
	/// Clears the container of all items, deleting them
	/// </summary>
	void Clear() override
	{
		// Delete and null all non-skippable items
		for (auto iter = mData.cbegin(); iter != mData.cend();)
		{
			if (iter->second != nullptr && !SkipItem(iter->second))
			{
				delete iter->second;
				iter = mData.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	/// <summary>
	/// Clears the data of the container without deleting it. 
	/// Only use this if pointers to the data exist elsewhere.
	/// </summary>
	void ClearData()
	{
		mData.clear();
	}

	/// <summary>
	/// Returns the number of items that will be written from the container
	/// </summary>
	/// <returns>Number of writable items</returns>
	unsigned int WriteCount() override
	{
		unsigned int count = 0;
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
			if (iter->second != nullptr && !SkipItem(iter->second))
				count++;
		return count;
	}

	/// <summary>
	/// Returns the number of elements in the container
	/// </summary>
	/// <returns>Number of elements in the container</returns>
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
	/// Returns a unique name, incrementing the existing name until it is unique
	/// </summary>
	/// <param name="name">Starting name</param>
	/// <param name="scope">Namespace of the item</param>
	/// <returns>New unique name</returns>
	virtual std::string GetUniqueName(const std::string& name)
	{
		std::string uniqueName = name;
		while (mData.contains(uniqueName))
		{
			uniqueName = IncrementName(uniqueName, 1);
		}
		return uniqueName;
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
	/// Returns whether the given item is deletable (ie. not a default element or internal)
	/// </summary>
	/// <param name="name">Item to consider</param>
	/// <returns>Whether the item can be deleted safely</returns>
	virtual bool IsDeleteable(T* item)
	{
		return true;
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
	/// <param name="replace">Whether to replace duplicate items (true) or rename the incoming item (false)</param>
	/// <param name="select">Whether to select the item after adding it</param>
	/// <returns>The element stored in the container</returns>
	virtual T* Add(const std::string& name, T* item, bool replace, bool select = false)
	{
		// If the item is new, add it
		if (!mData.contains(name))
			mData.emplace(name, item);
		// If the item isn't new, replace the existing item
		else if (replace)
			mData[name] = item;
		// If the item isn't new and shouldn't be replaced, rename the item
		else
			mData.emplace(GetUniqueName(name), item);

		// If the item added was the first item, select it
		if (select || mData.size() == 1)
			Select(name);
		return item;
	}

	/// <summary>
	/// Attempts to delete the given item. Does nothing if the item is not deleteable or is null.
	/// </summary>
	/// <param name="item">Item to try to delete</param>
	/// <returns>Whether the item was deleted or not</returns>
	virtual bool TryDelete(T* item)
	{
		if (item == nullptr || !IsDeleteable(item))
			return false;
		return Remove(item);
	}

	/// <summary>
	/// Removes the given item from the container
	/// </summary>
	/// <param name="name">Name of item to remove</param>
	/// <returns>Whether the item was removed or not</returns>
	virtual bool Remove(const std::string& name)
	{
		if (mData.contains(name))
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
		if (mData.contains(name))
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
			if (iter->second == item)
				return iter->first;
		return "";
	}

	/// <summary>
	/// Returns whether the container contains an item with the given name
	/// </summary>
	/// <param name="name">Name of the item</param>
	/// <returns>Whether the item exists</returns>
	virtual bool Contains(const std::string& name)
	{
		return mData.contains(name);
	}

	/// <summary>
	/// Renames the given item to the new name, or the new name with appended numbers
	/// </summary>
	/// <param name="name">Name of the item to rename</param>
	/// <param name="newName">Name to rename to</param>
	/// <returns>Whether the item was renamed</returns>
	virtual bool Rename(const std::string& name, const std::string& newName)
	{
		// Don't rename an item that doesn't exist
		if (!Contains(name))
			return false;

		// Validate name
		std::string itemName = GetUniqueName(newName);

		// Rename item and replace it into the data
		T* item = mData[name];
		RenameItem(item, itemName);
		mData.erase(name);
		mData.emplace(itemName, item);
		return true;
	}

	/// <summary>
	/// Returns a reference to the data stored in the container.
	/// </summary>
	/// <returns>Data of the container</returns>
	virtual const std::unordered_map<std::string, T*>& Data()
	{
		return mData;
	}

	/// <summary>
	/// Clears all of the container, deleting its elements
	/// </summary>
	~IContainer()
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
			if (iter->second != nullptr)
				delete iter->second;
		mData.clear();
	}
};