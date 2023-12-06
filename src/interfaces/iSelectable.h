#pragma once
#include "interfaces/iNameable.h"

/// <summary>
/// Enum for which kind of selectable the class is
/// </summary>
enum class SelectableType
{
	NONE = 0,
	MATERIAL,
	TEXTURE,
	CAMERA,
	LIGHT,
	MESH,
	NODE,
	ENTITY
};

/// <summary>
/// Interface for all items that can be selected to be viewed in the inspector
/// </summary>
class ISelectable : public INameable
{
protected:
	/// <summary>
	/// Type of the selectable
	/// </summary>
	SelectableType mSelectableType = SelectableType::NONE;
public:
	/// <summary>
	/// Returns the type of the selectable
	/// </summary>
	/// <returns>Type of the selectable</returns>
	virtual SelectableType GetSelectableType()
	{
		return mSelectableType;
	}

	/// <summary>
	/// Sets the type of the selectable
	/// </summary>
	/// <param name="type">Type of selectable</param>
	explicit ISelectable(SelectableType type = SelectableType::NONE)
		: mSelectableType(type)
	{

	}
};