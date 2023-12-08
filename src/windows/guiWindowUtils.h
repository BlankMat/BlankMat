#pragma once
#include "glIncludes.h"
#include "interfaces/iContainer.h"

class GUIWindowUtils
{
public:
	const static inline float TEXTURE_SIZE = 256.0f;
	const static inline std::vector<std::string> TEXTURE_TYPES = { "Diffuse", "Ambient", "Specular", "Normal", "Height", "Alpha" };

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

	static void ColorEdit(const std::string& label, glm::vec3& color, std::string& selColor, float spacing)
	{
		float size = ImGui::GetTextLineHeight();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		GUIWindowUtils::DrawColor(color, pos, size);
		ImGui::Dummy(ImVec2(size, size));
		ImGui::SameLine();
		GUIWindowUtils::Deselectable(label, selColor, label);
		if (selColor == label)
			color = GUIWindowUtils::ColorPicker("##" + label, color);
	}

	static void TextureEdit(Texture* texture)
	{
		int type = (int)texture->mType;
		texture->SetName(GUIWindowUtils::InputText("Name", texture->GetScopedName()));
		GUIWindowUtils::InputText("Path", texture->mDir);
		texture->mType = (TextureType)GUIWindowUtils::Dropdown("Type", type, TEXTURE_TYPES);
		GUIWindowUtils::Image(texture->mID, TEXTURE_SIZE);
		ImGui::Separator();
	}

	static bool TextureSelect(const std::string& name, Texture* texture, Texture*& selection)
	{
		GUIWindowUtils::Image((texture != nullptr ? texture->mID : 0), ImGui::GetTextLineHeight());
		ImGui::SameLine();

		bool wasPressed = false;
		GUIWindowUtils::Deselectable(name, selection, texture, &wasPressed);
		return wasPressed;
	}

	static bool MaterialSelect(const std::string& name, Material* material, Material*& selection, float spacing)
	{
		float size = ImGui::GetTextLineHeight();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		GUIWindowUtils::DrawColor((material != nullptr ? material->mKD : glm::vec3(0.0f)), pos, size);
		pos.x += size + spacing;
		GUIWindowUtils::DrawColor((material != nullptr ? material->mKA : glm::vec3(0.0f)), pos, size);
		pos.x += size + spacing;
		GUIWindowUtils::DrawColor((material != nullptr ? material->mKS : glm::vec3(0.0f)), pos, size);
		ImGui::Dummy(ImVec2((size + spacing) * 3, size));
		ImGui::SameLine();

		bool wasPressed = false;
		GUIWindowUtils::Selectable(name, selection, material, &wasPressed);
		return wasPressed;
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
	static bool Selectable(const std::string& label, T& selValue, T thisValue, bool* wasPressed = nullptr, const glm::vec2& size = glm::vec2(0.0f))
	{
		bool isSelected = (selValue == thisValue);
		if (ImGui::Selectable(label.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(size.x, size.y)))
		{
			selValue = thisValue;
			if (wasPressed != nullptr)
				*wasPressed = true;
		}
		if (isSelected)
		{
			ImGui::SetItemDefaultFocus();
		}
		return isSelected;
	}
	
	template<typename T>
	static bool Deselectable(const std::string& label, T*& selValue, T* thisValue, bool* wasPressed = nullptr, const glm::vec2& size = glm::vec2(0.0f))
	{
		bool isSelected = (selValue == thisValue);
		if (ImGui::Selectable(label.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(size.x, size.y)) && wasPressed != nullptr)
			*wasPressed = true;

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

	template<typename T>
	static bool InputName(const std::string& label, const std::string& prevValue, std::string& curValue, IContainer<T>* container, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		bool enterPressed = false;
		curValue = GUIWindowUtils::InputText((label + "##ItemName").c_str(), curValue, &enterPressed, flags);

		// Don't allow inputting a name that already exists
		if (curValue != prevValue && container->Contains(curValue))
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::Text((label + " with name " + curValue + " already exists.").c_str());
			ImGui::PopStyleColor();
			return false;
		}
		// If the text is valid and enter was pressed
		else if (curValue != "" && enterPressed)
		{
			return true;
		}
		// If enter was not pressed, return false
		return false;
	}

	static std::string InputText(const std::string& label, std::string value, bool* enterPressed = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		bool enter = ImGui::InputText(label.c_str(), &value, flags | ImGuiInputTextFlags_EnterReturnsTrue);
		if (enterPressed != nullptr)
			*enterPressed = enter;
		return value;
	}

	static int InputInt(const std::string& label, int value, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		ImGui::InputInt(label.c_str(), &value, 0, 0, flags);
		return value;
	}

	static float InputFloat(const std::string& label, float value, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		ImGui::InputFloat(label.c_str(), &value, 0.0f, 0.0f, "%.3f", flags);
		return value;
	}

	static glm::vec3 InputVec3(const std::string& label, glm::vec3 value, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		float valueInput[3] = { value.x, value.y, value.z };
		ImGui::InputFloat3(label.c_str(), valueInput, "%.3f", flags);
		return Vec3FromFloats(valueInput);
	}

	static glm::mat3 InputMat3(const std::string& label, const glm::mat3& value, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		float valueInput1[3] = { value[0].x, value[1].x, value[2].x };
		float valueInput2[3] = { value[0].y, value[1].y, value[2].y };
		float valueInput3[3] = { value[0].z, value[1].z, value[2].z };
		ImGui::InputFloat3(label.c_str(), valueInput1, "%.3f", flags);
		ImGui::InputFloat3(("##1" + label).c_str(), valueInput2, "%.3f", flags);
		ImGui::InputFloat3(("##2" + label).c_str(), valueInput3, "%.3f", flags);
		return Mat3FromFloats(valueInput1, valueInput2, valueInput3);
	}

	static glm::mat4 InputMat4(const std::string& label, const glm::mat4& value, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		float valueInput1[4] = { value[0].x, value[1].x, value[2].x, value[3].x };
		float valueInput2[4] = { value[0].y, value[1].y, value[2].y, value[3].y };
		float valueInput3[4] = { value[0].z, value[1].z, value[2].z, value[3].z };
		float valueInput4[4] = { value[0].w, value[1].w, value[2].w, value[3].w };
		ImGui::InputFloat4(label.c_str(), valueInput1, "%.3f", flags);
		ImGui::InputFloat4(("##1" + label).c_str(), valueInput2, "%.3f", flags);
		ImGui::InputFloat4(("##2" + label).c_str(), valueInput3, "%.3f", flags);
		ImGui::InputFloat4(("##3" + label).c_str(), valueInput4, "%.3f", flags);
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