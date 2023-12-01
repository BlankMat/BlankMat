#pragma once
#include "glIncludes.h"

class GUIWindowUtils
{
public:
	static bool Popup(const std::string& label, bool& showPopup)
	{
		if (ImGui::Button(("Open " + label).c_str()))
		{
			ImGui::OpenPopup(label.c_str());
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal(label.c_str()))
		{
			ImGui::SetItemDefaultFocus();
			if (ImGui::Button(("Close " + label).c_str()))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	
	static int Dropdown(const std::string& label, int& index, const std::vector<std::string>& options)
	{
		if (ImGui::BeginListBox(label.c_str()))
		{
			for (int i = 0; i < options.size(); i++)
			{
				GUIWindowUtils::Selectable(options[i], index, i);
			}
			ImGui::EndListBox();
		}
		return index;
	}

	static void Image(unsigned int textureID, const glm::vec2& size, const glm::vec3& borderColor = glm::vec3(0, 0, 0), float borderAlpha = 1.0f)
	{
		ImVec4 border = ImVec4(borderColor.x, borderColor.y, borderColor.z, borderAlpha);
#pragma warning(suppress: 4312)
		ImGui::Image((void*)textureID, ImVec2(size.x, size.y), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), border);
	}

	static void Image(unsigned int textureID, float size, const glm::vec3& borderColor = glm::vec3(0, 0, 0), float borderAlpha = 1.0f)
	{
		Image(textureID, glm::vec2(size, size), borderColor, borderAlpha);
	}

	static bool ImageButton(unsigned int textureID, const glm::vec2& size, const glm::vec3& tintColor = glm::vec3(0, 0, 0), float tintAlpha = 1.0f)
	{
		ImVec4 tint = ImVec4(tintColor.x, tintColor.y, tintColor.z, tintAlpha);
#pragma warning(suppress: 4312)
		return ImGui::ImageButton((void*)textureID, ImVec2(size.x, size.y), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), tint);
	}

	static bool ImageButton(unsigned int textureID, float size, const glm::vec3& tintColor = glm::vec3(0, 0, 0), float tintAlpha = 1.0f)
	{
		return ImageButton(textureID, glm::vec2(size, size), tintColor, tintAlpha);
	}

	template<typename T>
	static bool Selectable(const std::string& label, T& selValue, T thisValue)
	{
		bool isSelected = (selValue == thisValue);
		if (ImGui::Selectable(label.c_str(), &isSelected))
			selValue = thisValue;
		if (isSelected)
			ImGui::SetItemDefaultFocus();
		return isSelected;
	}
	
	template<typename T>
	static bool Deselectable(const std::string& label, T*& selValue, T* thisValue)
	{
		bool isSelected = (selValue == thisValue);
		ImGui::Selectable(label.c_str(), &isSelected);

		// If the item is clicked, toggle selection
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			isSelected = !isSelected;
			selValue = isSelected ? thisValue : nullptr;
		}

		if (isSelected)
			ImGui::SetItemDefaultFocus();
		return isSelected;
	}

	static bool Deselectable(const std::string& label, std::string& selValue, std::string thisValue)
	{
		bool isSelected = (selValue == thisValue);
		ImGui::Selectable(label.c_str(), &isSelected);

		// If the item is clicked, toggle selection
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			isSelected = !isSelected;
			selValue = isSelected ? thisValue : "";
		}

		if (isSelected)
			ImGui::SetItemDefaultFocus();
		return isSelected;
	}

	static bool Checkbox(const std::string& label, bool value)
	{
		ImGui::Checkbox(label.c_str(), &value);
		return value;
	}
	
	static std::string InputText(const std::string& label, std::string value)
	{
		ImGui::InputText(label.c_str(), &value);
		return value;
	}

	static int InputInt(const std::string& label, int value)
	{
		ImGui::InputInt(label.c_str(), &value, 0);
		return value;
	}

	static float InputFloat(const std::string& label, float value)
	{
		ImGui::InputFloat(label.c_str(), &value);
		return value;
	}

	static glm::vec3 InputVec3(const std::string& label, glm::vec3 value)
	{
		float valueInput[3] = { value.x, value.y, value.z };
		ImGui::InputFloat3(label.c_str(), valueInput);
		return Vec3FromFloats(valueInput);
	}

	static glm::mat3 InputMat3(const std::string& label, const glm::mat3& value)
	{
		float valueInput1[3] = { value[0].x, value[1].x, value[2].x };
		float valueInput2[3] = { value[0].y, value[1].y, value[2].y };
		float valueInput3[3] = { value[0].z, value[1].z, value[2].z };
		ImGui::InputFloat3(label.c_str(), valueInput1);
		ImGui::InputFloat3(("##1" + label).c_str(), valueInput2);
		ImGui::InputFloat3(("##2" + label).c_str(), valueInput3);
		return Mat3FromFloats(valueInput1, valueInput2, valueInput3);
	}

	static glm::mat4 InputMat4(const std::string& label, const glm::mat4& value)
	{
		float valueInput1[4] = { value[0].x, value[1].x, value[2].x, value[3].x };
		float valueInput2[4] = { value[0].y, value[1].y, value[2].y, value[3].y };
		float valueInput3[4] = { value[0].z, value[1].z, value[2].z, value[3].z };
		float valueInput4[4] = { value[0].w, value[1].w, value[2].w, value[3].w };
		ImGui::InputFloat4(label.c_str(), valueInput1);
		ImGui::InputFloat4(("##1" + label).c_str(), valueInput2);
		ImGui::InputFloat4(("##2" + label).c_str(), valueInput3);
		ImGui::InputFloat4(("##3" + label).c_str(), valueInput4);
		return Mat4FromFloats(valueInput1, valueInput2, valueInput3, valueInput4);
	}

	static void DrawColor(const glm::vec3& color, ImVec2 pos, float size)
	{
		ImU32 colorU = ImGui::GetColorU32(ImVec4(color.r, color.g, color.b, 1.0f));
		ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size, pos.y + size), colorU);
	}

	static glm::vec3 ColorPicker(const std::string& label, const glm::vec3& value)
	{
		float valueInput[3] = { value.x, value.y, value.z };
		ImGui::ColorPicker3(label.c_str(), valueInput);
		return Vec3FromFloats(valueInput);
	}
};