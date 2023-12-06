#pragma once
#include "interfaces/iGUIWindow.h"
#include "guiWindowUtils.h"
#include "tools/state.h"
#include "rendering/scene.h"

class GUIInspectorWindow : public IGUIWindow
{
protected:
	State* mState = nullptr;
	Scene* mScene = nullptr;
	std::string mSelColor = "";

	void DisplayMesh(Mesh* element)
	{
		// Don't show null entities
		if (element == nullptr)
			return;

		// Change name of selection
		element->SetName(GUIWindowUtils::InputText("Mesh", element->GetScopedName()));

		// Position
		element->SetPos(GUIWindowUtils::InputVec3("Position", element->GetPos()));

		// Rotation
		element->SetRot(GUIWindowUtils::InputVec3("Rotation", element->GetRot()));

		// Scale
		element->SetScale(GUIWindowUtils::InputVec3("Scale", element->GetScale()));

		// Enabled
		element->Enable(GUIWindowUtils::Checkbox("Enabled", element->IsEnabled()));
	}

	void DisplayNode(Node* element)
	{
		// Don't show null entities
		if (element == nullptr)
			return;

		// Change name of selection
		element->SetName(GUIWindowUtils::InputText("Node", element->GetScopedName()));

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

		// Select light type
		ImGui::Text("Light");
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

		ImGui::Text("Camera");
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
		GUIWindowUtils::TextureEdit(element);
	}

	void DisplayMaterial(Material* element)
	{
		if (element == nullptr)
			return;

		Texture* selTexture = mScene->GetTextures()->GetSelectedItem();
		element->GetScopedName() = GUIWindowUtils::InputText("Material", element->GetScopedName());

		float spacing = 15.0f;
		GUIWindowUtils::ColorEdit("Diffuse", element->mKD, mSelColor, spacing);
		GUIWindowUtils::ColorEdit("Ambient", element->mKA, mSelColor, spacing);
		GUIWindowUtils::ColorEdit("Specular", element->mKS, mSelColor, spacing);
		GUIWindowUtils::ColorEdit("Emissive", element->mKE, mSelColor, spacing);

		element->mNS = GUIWindowUtils::InputFloat("Specular Exponent", element->mNS);
		element->mNI = GUIWindowUtils::InputFloat("Index of Refraction", element->mNI);
		element->mD = GUIWindowUtils::InputFloat("Alpha", element->mD);
		element->mIllum = GUIWindowUtils::InputInt("Illumination Mode", element->mIllum);

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
				ImGui::Text(("Selected Type: " + std::to_string(sel != nullptr ? (int)sel->GetSelectableType() : -1)).c_str());

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