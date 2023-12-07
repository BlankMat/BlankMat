#pragma once
#include "interfaces/iGUIWindow.h"
#include "guiWindowUtils.h"
#include "interaction/state.h"
#include "rendering/scene.h"

class GUIInspectorWindow : public IGUIWindow
{
protected:
	State* mState = nullptr;
	Scene* mScene = nullptr;
	std::string mSelColor = "";

	std::string mNameInEdit = "";
	ISelectable* mElementInEdit = nullptr;

	Material* mNewMat = nullptr;
	Texture* mNewTexture = nullptr;

	const static inline float TEXTURE_SIZE = 256.0f;
	const static inline std::vector<std::string> TEXTURE_TYPES = { "Diffuse", "Ambient", "Specular", "Normal", "Height", "Alpha" };

	static bool InputName(const std::string& label, const std::string& prevValue, std::string& curValue, Node* node, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		bool enterPressed = false;
		curValue = GUIWindowUtils::InputText((label + "##ItemName").c_str(), curValue, &enterPressed, flags);

		// Don't allow inputting a name that already exists
		if (curValue != prevValue && node->FindNode(curValue) != nullptr)
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

	static bool RenameItem(const std::string& label, IContainer<ISelectable>* container, ISelectable* element, std::string& name)
	{
		if (name == "")
			name = element->GetScopedName();

		std::string newName = GUIWindowUtils::InputText(label, name);
		if (newName == name && newName != element->GetScopedName())
		{
			// Don't allow creating of items with duplicate names
			if (container->Contains(newName))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				ImGui::Text((label + " with name " + newName + " already exists.").c_str());
				ImGui::PopStyleColor();
			}
			// If there is any text, display the add button
			else if (newName != "")
			{
				container->Rename(element->GetScopedName(), newName);
				name = "";
			}
		}
	}

	void DisplayMesh(Mesh* element)
	{
		// Don't show null entities
		if (element == nullptr)
			return;

		// Change name of selection
		if (GUIWindowUtils::InputName("Mesh", element->GetScopedName(), mNameInEdit, mScene->GetMeshes()))
		{
			mScene->GetMeshes()->Rename(element->GetScopedName(), mNameInEdit);
		}

		// Position
		element->SetPos(GUIWindowUtils::InputVec3("Position", element->GetPos()));

		// Rotation
		element->SetRot(GUIWindowUtils::InputVec3("Rotation", element->GetRot()));

		// Scale
		element->SetScale(GUIWindowUtils::InputVec3("Scale", element->GetScale()));

		// Enabled
		element->Enable(GUIWindowUtils::Checkbox("Enabled", element->IsEnabled()));

		// Material
		GUIWindowUtils::MaterialSelect("Material: " + element->GetMaterial()->GetScopedName(), element->GetMaterial(), mNewMat, 5.0f);
		if (mNewMat != nullptr)
		{
			mNewMat = nullptr;
			mState->materialInEdit = &mNewMat;
		}

		// Update material
		if (mState->materialInEdit != nullptr && *mState->materialInEdit != nullptr)
		{
			element->SetMaterial(*mState->materialInEdit);
			mState->materialInEdit = nullptr;
		}
	}

	void DisplayNode(Node* element)
	{
		// Don't show null entities
		if (element == nullptr)
			return;

		// Change name of selection
		if (InputName("Node", element->GetScopedName(), mNameInEdit, mScene->GetRootNode()))
		{
			element->Rename(mNameInEdit, true);
		}

		// Position
		element->SetPos(GUIWindowUtils::InputVec3("Position", element->GetPos()));

		// Rotation
		element->SetRot(GUIWindowUtils::InputVec3("Rotation", element->GetRot()));

		// Scale
		element->SetScale(GUIWindowUtils::InputVec3("Scale", element->GetScale()));

		// Enabled
		element->Enable(GUIWindowUtils::Checkbox("Enabled", element->IsEnabled()));
	}

	void DisplayEntity(IEntity* element)
	{
		// Don't show null entities
		if (element == nullptr)
			return;

		// Change name of selection
		element->SetName(GUIWindowUtils::InputText("Entity", element->GetScopedName()));

		// Position
		element->SetPos(GUIWindowUtils::InputVec3("Position", element->GetPos()));

		// Rotation
		element->SetRot(GUIWindowUtils::InputVec3("Rotation", element->GetRot()));

		// Scale
		element->SetScale(GUIWindowUtils::InputVec3("Scale", element->GetScale()));

		// Enabled
		element->Enable(GUIWindowUtils::Checkbox("Enabled", element->IsEnabled()));
	}

	void DisplayLight(Light* element)
	{
		if (element == nullptr)
			return;

		// Change name of selection
		if (GUIWindowUtils::InputName("Light", element->GetScopedName(), mNameInEdit, mScene->GetLights()))
		{
			mScene->GetLights()->Rename(element->GetScopedName(), mNameInEdit);
		}

		// Select light type
		LightType type = element->GetType();
		if (ImGui::BeginListBox("Type"))
		{
			GUIWindowUtils::Selectable("Point", type, LightType::POINT);
			GUIWindowUtils::Selectable("Directional", type, LightType::DIR);
			GUIWindowUtils::Selectable("Spot", type, LightType::SPOT);
			element->SetType(type);
			ImGui::EndListBox();
		}

		// Light position
		if (type == LightType::SPOT || type == LightType::POINT)
		{
			element->SetOffset(GUIWindowUtils::InputVec3("Position", element->GetOffset()));
		}

		// Light direction
		if (type == LightType::SPOT || type == LightType::DIR)
		{
			element->SetDir(GUIWindowUtils::InputVec3("Direction", element->GetDir()));
		}

		// Light Diffuse, Ambient, and Specular
		element->SetKD(GUIWindowUtils::InputFloat("Diffuse", element->GetKD()));
		element->SetKA(GUIWindowUtils::InputFloat("Ambient", element->GetKA()));
		element->SetKS(GUIWindowUtils::InputFloat("Specular", element->GetKS()));

		// Light gamma
		element->SetGamma(GUIWindowUtils::Checkbox("Gamma", element->GetGamma()));

		// Show attributes of point and spotlights
		if (type == LightType::POINT || type == LightType::SPOT)
		{
			// Range coefficient
			element->SetRange(GUIWindowUtils::InputFloat("Range", element->GetRange()));
		}
		// Show attributes of spotlight
		if (type == LightType::SPOT)
		{
			// Inner radius
			element->SetSpotInnerRadius(GUIWindowUtils::InputFloat("Inner Radius", element->GetSpotInnerRadius()));
			// Outer radius
			element->SetSpotOuterRadius(GUIWindowUtils::InputFloat("Outer Radius", element->GetSpotOuterRadius()));
		}

		// Light color
		element->SetBaseColor(GUIWindowUtils::ColorPicker("Light Color", element->GetBaseColor()));
	}

	void DisplayCamera(Camera* element)
	{
		if (element == nullptr)
			return;

		if (GUIWindowUtils::InputName("Camera", element->GetScopedName(), mNameInEdit, mScene->GetCameras()))
		{
			mScene->GetCameras()->Rename(element->GetScopedName(), mNameInEdit);
		}

		element->GetTarget().Display();
		element->GetPivot().Display();
		element->GetOrthSize().Display();
		element->GetFOV().Display();
		element->GetNearClip().Display();
		element->GetFarClip().Display();
		element->IsPerspective().Display();
		element->IsRotatingAroundPivot().Display();
	}

	void DisplayTexture(Texture* element)
	{
		if (GUIWindowUtils::InputName("Texture", element->GetScopedName(), mNameInEdit, mScene->GetTextures()))
		{
			mScene->GetTextures()->Rename(element->GetScopedName(), mNameInEdit);
		}
		GUIWindowUtils::InputText("Path", element->mDir);

		int type = (int)element->mType;
		element->mType = (TextureType)GUIWindowUtils::Dropdown("Type", type, TEXTURE_TYPES);

		GUIWindowUtils::Image(element->mID, TEXTURE_SIZE);
	}

	void DisplayMaterial(Material* element)
	{
		if (element == nullptr)
			return;

		if (GUIWindowUtils::InputName("Material", element->GetScopedName(), mNameInEdit, mScene->GetMaterials()))
		{
			mScene->GetMaterials()->Rename(element->GetScopedName(), mNameInEdit);
		}

		float spacing = 15.0f;
		GUIWindowUtils::ColorEdit("Diffuse", element->mKD, mSelColor, spacing);
		GUIWindowUtils::ColorEdit("Ambient", element->mKA, mSelColor, spacing);
		GUIWindowUtils::ColorEdit("Specular", element->mKS, mSelColor, spacing);
		GUIWindowUtils::ColorEdit("Emissive", element->mKE, mSelColor, spacing);

		element->mNS = GUIWindowUtils::InputFloat("Specular Exponent", element->mNS);
		element->mNI = GUIWindowUtils::InputFloat("Index of Refraction", element->mNI);
		element->mD = GUIWindowUtils::InputFloat("Alpha", element->mD);
		element->mIllum = GUIWindowUtils::InputInt("Illumination Mode", element->mIllum);

		Texture* selTexture = mScene->GetTextures()->GetSelectedItem();
		GUIWindowUtils::TextureSelect("Diffuse", element->mMapKD, selTexture);
		GUIWindowUtils::TextureSelect("Ambient", element->mMapKA, selTexture);
		GUIWindowUtils::TextureSelect("Specular", element->mMapKS, selTexture);
		GUIWindowUtils::TextureSelect("Normal", element->mMapBump, selTexture);
		GUIWindowUtils::TextureSelect("Height", element->mMapNS, selTexture);
		GUIWindowUtils::TextureSelect("Alpha", element->mMapD, selTexture);

		if (selTexture != mScene->GetTextures()->GetSelectedItem())
		{
			mScene->GetTextures()->Select(selTexture);
		}
	}

public:
	void Draw() override
	{
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Inspector", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Only show options if selected
			ISelectable* sel = mState->GetSel()->GetSelectedElement();
			if (sel != nullptr)
			{
				ImGui::Text(("Selected Type: " + SelectableTypeToString(sel->GetSelectableType())).c_str());


				// If changing the element selected, change the name in edit
				if (sel != mElementInEdit)
				{
					mElementInEdit = sel;
					mNameInEdit = sel->GetScopedName();
				}

				switch (sel->GetSelectableType())
				{
				case SelectableType::MATERIAL:
					DisplayMaterial(dynamic_cast<Material*>(sel));
					break;
				case SelectableType::TEXTURE:
					DisplayTexture(dynamic_cast<Texture*>(sel));
					break;
				case SelectableType::CAMERA:
					DisplayCamera(dynamic_cast<Camera*>(sel));
					break;
				case SelectableType::LIGHT:
					DisplayLight(dynamic_cast<Light*>(sel));
					break;
				case SelectableType::MESH:
					DisplayMesh(dynamic_cast<Mesh*>(sel));
					break;
				case SelectableType::NODE:
					DisplayNode(dynamic_cast<Node*>(sel));
					break;
				case SelectableType::ENTITY:
					DisplayEntity(dynamic_cast<IEntity*>(sel));
					break;
				case SelectableType::NONE:
				default:
					break;
				}
			}
			else
			{
				ImGui::Text("Selected: None");
			}
		}
		ImGui::End();
	}

	GUIInspectorWindow(State* state, Scene* scene, bool isEnabled)
		: mState(state), mScene(scene)
	{
		mType = GUI::INSPECTOR;
		mIsEnabled = isEnabled;
	}
};