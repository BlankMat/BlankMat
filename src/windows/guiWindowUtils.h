#pragma once
#include "glIncludes.h"

class GUIWindowUtils
{
public:
	template<typename T>
	static bool Selectable(std::string label, T& selValue, T thisValue)
	{
		bool isSelected = (selValue == thisValue);
		if (ImGui::Selectable(label.c_str(), &isSelected))
			selValue = thisValue;
		if (isSelected)
			ImGui::SetItemDefaultFocus();
		return isSelected;
	}

	static bool Checkbox(std::string label, bool value)
	{
		ImGui::Checkbox(label.c_str(), &value);
		return value;
	}
	
	static std::string InputText(std::string label, std::string value)
	{
		ImGui::InputText(label.c_str(), &value);
		return value;
	}

	static int InputInt(std::string label, int value)
	{
		ImGui::InputInt(label.c_str(), &value);
		return value;
	}

	static float InputFloat(std::string label, float value)
	{
		ImGui::InputFloat(label.c_str(), &value);
		return value;
	}

	static glm::vec3 InputVec3(std::string label, glm::vec3 value)
	{
		float valueInput[3] = { value.x, value.y, value.z };
		ImGui::InputFloat3(label.c_str(), valueInput);
		return Vec3FromFloats(valueInput);
	}

	static void DrawColor(glm::vec3 color, ImVec2 pos, float size)
	{
		ImU32 colorU = ImGui::GetColorU32(ImVec4(color.r, color.g, color.b, 1.0f));
		ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size, pos.y + size), colorU);
	}

	static glm::vec3 ColorPicker(std::string label, glm::vec3 value)
	{
		float valueInput[3] = { value.x, value.y, value.z };
		ImGui::ColorPicker3(label.c_str(), valueInput);
		return Vec3FromFloats(valueInput);
	}
};