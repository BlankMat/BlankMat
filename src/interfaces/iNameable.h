#pragma once
#include "utils.h"
#include <string>

/// <summary>
/// An object that keeps track of its name and scope
/// </summary>
class INameable
{
protected:
	/// <summary>
	/// Name of the object
	/// </summary>
	std::string mName = "";

	/// <summary>
	/// Scope of the object
	/// </summary>
	std::string mScope = "";

public:
	/// <summary>
	/// Renames the object, setting the scope to be the scoped part of the name
	/// </summary>
	/// <param name="newName">New name to rename to</param>
	/// <param name="includeScope">Whether to incldude scope as part of the renaming</param>
	virtual void Rename(const std::string& newName, bool includeScope = true)
	{
		// If not including the scope, simply rename the object
		if (!includeScope)
		{
			mName = newName;
			return;
		}

		// If the name is scoped, store the scope separately
		if (IsScoped(newName))
		{
			mScope = UnscopeScope(newName);
			mName = UnscopeName(newName);
			return;
		}

		// If there is no scope, set scope to null
		mScope = "";
		mName = newName;
	}

	/// <summary>
	/// Sets the name of the object. Note: Does not set the scope variable.
	/// If renaming based on raw input, use Rename() instead to set the scope.
	/// </summary>
	/// <param name="name">Raw name of the object</param>
	virtual void SetName(const std::string& name)
	{
		if (mName == name)
			return;
		mName = name;
	}

	/// <summary>
	/// Sets the scope of the object
	/// </summary>
	/// <param name="scope">Scope to set the object to</param>
	virtual void SetScope(const std::string& scope)
	{
		if (mScope == scope)
			return;
		mScope = scope;
	}

	/// <summary>
	/// Returns the unscoped name of the object
	/// </summary>
	/// <returns>Name of the object</returns>
	std::string GetUnscopedName()
	{
		return mName;
	}

	/// <summary>
	/// Returns the scope of the object
	/// </summary>
	/// <returns>Scope of the object</returns>
	std::string GetScope()
	{
		return mScope;
	}

	/// <summary>
	/// Returns the scoped name of the object
	/// </summary>
	/// <returns>Scoped name of the object</returns>
	std::string GetScopedName()
	{
		return Scope(mName, mScope);
	}

	/// <summary>
	/// Initializes the nameable object with the given name and scope
	/// </summary>
	/// <param name="name">Name of the object</param>
	/// <param name="scope">Scope of the object (if any)</param>
	void InitName(const std::string& name, const std::string& scope = "")
	{
		mName = name;
		mScope = scope;
	}

	/// <summary>
	/// Creates a default nameable object without name or scope
	/// </summary>
	explicit INameable()
	{

	}
};