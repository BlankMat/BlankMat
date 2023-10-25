#pragma once
#include "glIncludes.h"
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
public:
	/// <summary>
	/// Returns the number of elements in the
	/// </summary>
	/// <returns></returns>
	unsigned int Count()
	{
		return mData.size();
	}

	/// <summary>
	/// Selects the given item. Does nothing if the item is not part of the container.
	/// </summary>
	/// <param name="name">Name of item to select</param>
	/// <returns>Whether the item was selected or not</returns>
	virtual bool Select(const std::string& name)
	{
		T* item = Get(name);
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
		std::string name = Get(item);
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
	virtual T* Get(const std::string& name)
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
	virtual const std::string Get(T* item)
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
	/// Returns a reference to the data stored in the container.
	/// WARNING: Only use this to iterate over the data, not to modify it
	/// </summary>
	/// <returns>Data of the container</returns>
	virtual const std::unordered_map<std::string, T*>& Data()
	{
		return mData;
	}
};