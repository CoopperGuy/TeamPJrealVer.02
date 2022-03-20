#include "stdafx.h"
#include "..\Public\Inspector.h"
#include "GameObject.h"
#include "VIBuffer_LineSphere.h"
#include "VIBuffer_RectUI.h"
#include "VIBuffer_RectInstance.h"
#include "VIBuffer_RectEffect.h"
#include "VIBuffer_Terrain.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "EmptyGameObject.h"
#include "EmptyMapObject.h"
#include "EmptyEffect.h"
#include "RectTransform.h"
#include "Engine.h"
#include "Log.h"
#include "EmptyUI.h"
#include "Light.h"
#include "InstanceModel.h"
#include "Layer.h"
#include "BasicCollider.h"
#include "Shop.h"

USING(Tool)

static vector<string> idTypes = { "Player","WEAPONE", "MONSTER","MONSTER_ATK","PLAYER_EFFECT", "MONSTER_EFFECT", "EFFECT","NPC","IDEND"};


CInspector::CInspector()
{
	Initialize();
}

void CInspector::Initialize()
{
	m_pGizmo = dynamic_cast<CGizmo*>(CEngine::GetInstance()->GetWindow("Gizmo"));
	//m_pGizmo = dynamic_pointer_cast<CGizmo>(m_pToolManager->GetWindow("Gizmo"));
}

void CInspector::Update()
{
	ImGui::Begin("Inspector");
	ImGui::Separator();

	if (g_pObjFocused)
	{
		/* Get GameObject Name and Change */
		char buf[64];
		sprintf_s(buf, g_pObjFocused->GetName().c_str());
		ImGui::InputText("##Name", buf, IM_ARRAYSIZE(buf));
		g_pObjFocused->SetName(string(buf));

		//ImGui::SameLine();
		ImGui::Text("Set isActivate");
		ImGui::SameLine();
		//_bool& active = g_pObjFocused->IsActive();
		//ImGui::Checkbox("##IsActive", &active);
		_bool active = g_pObjFocused->IsActive();
		ImGui::Checkbox("##IsActive", &active);
		g_pObjFocused->SetActive(active);

		ImGui::Separator();

		sprintf_s(buf, g_pObjFocused->GetLayer().c_str());
		ImGui::InputText("Layer", buf, IM_ARRAYSIZE(buf));
		g_pObjFocused->SetLayer(string(buf));

		ImGui::Separator();
		
		_bool Ins = g_pObjFocused->GetInstancing();
		ImGui::Checkbox("Has Instancing", &Ins);
		g_pObjFocused->SetInstacing(Ins);


		g_pObjFocused->SetActive(active);


		if (dynamic_cast<CEmptyGameObject*>(g_pObjFocused))
			UpdateGameObject();
		else if (dynamic_cast<CEmptyMapObject*>(g_pObjFocused))
			UpdateMapObject();
		else if (dynamic_cast<CEmptyEffect*>(g_pObjFocused))
			UpdateEffect();
		else
			UpdateUI();
	}

	ImGui::End();

}

void CInspector::LateUpdate()
{
}

void CInspector::UpdateGameObject()
{
	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");
	if (ImGui::BeginPopup("AddComponent"))
	{
		ImGui::Text("Collider");
		ImGui::Indent();
		if (ImGui::MenuItem("SphereCollider"))
		{
			/* Add Collider */
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_SphereCollider", "Com_Collider", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}
		if (ImGui::MenuItem("BoxCollider"))
		{
			/* Add Collider */
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_BoxCollider", "Com_Collider", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}
		if (ImGui::MenuItem("CapsuleCollider"))
		{
			/* Add Collider */
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_CapsuleCollider", "Com_Collider", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}
		if (ImGui::MenuItem("OBB"))
		{
			/* Add Collider */
			string com = "Com_OBB";
			_int i = static_cast<CEmptyGameObject*>(g_pObjFocused)->GetNumOBB();
			if (i != 0)
				com += to_string(i);
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_OBBCollider", com)))
				MSG_BOX("Failed to Com_OBB");
			static_cast<CEmptyGameObject*>(g_pObjFocused)->SetNumOBB(i + 1);
		}
		if (ImGui::MenuItem("AABB"))
		{
			/* Add Collider */
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_AABBCollider", "Com_AABB")))
				MSG_BOX("Failed to Com_AABB");
		}


		ImGui::Unindent();

		ImGui::Separator();

		if (ImGui::MenuItem("Terrain"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_VIBuffer_Terrain", "Com_VIBuffer", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		ImGui::Separator();

		if (ImGui::MenuItem("VIBuffer"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_VIBuffer_Rect", "Com_VIBuffer", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Model"))
		{
			CEngine::GetInstance()->CloneModel(g_pObjFocused, "", "", "", "", false, g_pObjFocused->GetComponent("Com_Transform"));
		}
		ImGui::Separator();

		ImGui::Text("Light");
		ImGui::Indent();
		LIGHTDESC desc;
		desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		desc.vAmbient = _float4(1.0f, 1.f, 1.f, 1.f);
		desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
		desc.fLightRange = 10.f;
		desc.fLightAngle = 20.f;
		if (ImGui::MenuItem("Directional"))
		{
			desc.eType = LIGHTDESC::LIGHT_DIRECTION;
			CComponent* pLight = CLight::Create(CEngine::GetInstance()->GetDevice(), CEngine::GetInstance()->GetDeviceContext(), desc, dynamic_cast<CTransform*>(g_pObjFocused->GetComponent("Com_Transform")));
			if (FAILED(g_pObjFocused->AddComponent("Com_Light", pLight)))
				MSG_BOX("Failed to AddComponent");
		}
		if (ImGui::MenuItem("Point"))
		{
			desc.eType = LIGHTDESC::LIGHT_POINT;
			CComponent* pLight = CLight::Create(CEngine::GetInstance()->GetDevice(), CEngine::GetInstance()->GetDeviceContext(), desc, dynamic_cast<CTransform*>(g_pObjFocused->GetComponent("Com_Transform")));
			if (FAILED(g_pObjFocused->AddComponent("Com_Light", pLight)))
				MSG_BOX("Failed to AddComponent");
		}
		if (ImGui::MenuItem("Spot"))
		{
			desc.eType = LIGHTDESC::LIGHT_SPOT;
			CComponent* pLight = CLight::Create(CEngine::GetInstance()->GetDevice(), CEngine::GetInstance()->GetDeviceContext(), desc, dynamic_cast<CTransform*>(g_pObjFocused->GetComponent("Com_Transform")));
			if (FAILED(g_pObjFocused->AddComponent("Com_Light", pLight)))
				MSG_BOX("Failed to AddComponent");
		}
		ImGui::Unindent();

		ImGui::Separator();

		if (ImGui::MenuItem("Navigation"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_Navigation", "Com_Navi", g_pObjFocused->GetComponent("Com_Navi"))))
				MSG_BOX("Failed to AddComponent");

			ImGui::Text("AddNavi");
		}

		ImGui::Separator();

		if (ImGui::MenuItem("NPCInfo"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_NPCInfo", "Com_NPCInfo", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		if (ImGui::MenuItem("Stat"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_Stat", "Com_Stat", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		if (ImGui::MenuItem("Shop"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_Shop", "Com_Shop", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		if (ImGui::MenuItem("Spawn"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_Spawn", "Com_Spawn", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		ImGui::EndPopup();
	}

	if (ImGui::Button("RemoveParent"))
	{
		g_pObjFocused->RemoveParent();
	}

	ImGui::Separator();

	DrawTransform();

	ImGui::Separator();

	DrawBuffer();

	ImGui::Separator();

	DrawModel();

	ImGui::Separator();

	DrawCollider();

	ImGui::Separator();

	DrawLight();

	ImGui::Separator();

	DrawNavi();

	ImGui::Separator();

	DrawNPCInfo();

	ImGui::Separator();

	DrawBasicCollider();

	ImGui::Separator();

	DrawStat();

	ImGui::Separator();

	DrawShop();

	ImGui::Separator();

	DrawSpawn();
}

void CInspector::UpdateMapObject()
{
	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");
	if (ImGui::BeginPopup("AddComponent"))
	{
		ImGui::Separator();

		if (ImGui::MenuItem("InstancingModel"))
		{
			////CEngine::GetInstance()->CloneModel(g_pObjFocused, "", "", "", "", false, g_pObjFocused->GetComponent("Com_Transform"));

			//////m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("GameObject_RollEffect"), TEXT("Layer_RollEffect"), &pos);
			//////if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_InstanceModel", "Com_Instacne", g_pObjFocused->GetComponent("Com_Instacne"))))
			//////	MSG_BOX("Failed to AddComponent");

			////char *		pMeshFilePath;
			////char *		pMeshFileName;
			////_tchar*		pShaderFilePath;
			////_fmatrix	PivotMatrix;
			////_int		_InstancingCount = 1;
			////_bool		bMeshCollider = false;
			////void*		pArg = nullptr;

			///*g_pObjFocused->*/
			//_matrix pivo = XMMatrixIdentity();

			//_matrix		ScaleMatrix, RotationMatrix, TranslationMatrix;
			//_matrix		ModelPivotMatrix;

			//ScaleMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
			////ScaleMatrix = XMMatrixScaling(2.f, 2.f, 1.f);
			//RotationMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f));

			//ModelPivotMatrix = ScaleMatrix/* * RotationMatrix*/;

			//_int tssemp = 1;

			////임시용으로 만들기
			//INSMODELDESC* temp = new INSMODELDESC;
			//temp->pMeshFilePath = "../../Assets/Meshes/Map/Housing/Box/";
			//temp->pMeshFileName = "1.fbx";
			//temp->pShaderFilePath = TEXT("../../Assets/Shader/Shader_ModelInstance.fx");
			//temp->PivotMatrix = ModelPivotMatrix;
			//temp->_InstancingCount = &tssemp;
			//temp->bMeshCollider = false;
			//temp->pArg = g_pObjFocused->GetComponent("Com_Transform");
			//if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_InstanceModel", "Com_InstanceModel", temp)))
			//	MSG_BOX("Failed to AddComponent");

			CEngine::GetInstance()->CloneModel(g_pObjFocused, "", "", "", "", false, g_pObjFocused->GetComponent("Com_Transform"));
		}
		ImGui::Separator();

		if (ImGui::MenuItem("Navigation"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_Navigation", "Com_Navi", g_pObjFocused->GetComponent("Com_Navi"))))
				MSG_BOX("Failed to AddComponent Navigation");

			ImGui::Text("AddNavi");
		}

		ImGui::EndPopup();
	}

	if (ImGui::Button("RemoveParent"))
	{
		g_pObjFocused->RemoveParent();
	}

	ImGui::Separator();

	DrawInstancing();

	ImGui::Separator();

	DrawNavi();

	ImGui::Separator();
	DrawTransform();
}

void CInspector::UpdateEffect()
{

	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");
	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("MeshEffect"))
		{
			CEngine::GetInstance()->CloneModel(g_pObjFocused, "", "", "../../Assets/Shader/Shader_Effect.fx", "", false, g_pObjFocused->GetComponent("Com_Transform"));
		}

		if (ImGui::MenuItem("TextureEffect"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_VIBuffer_RectEffect", "Com_VIBuffer", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		if (ImGui::MenuItem("PointInstance"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_VIBuffer_PointInstance", "Com_PointInstance", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		if (ImGui::MenuItem("OBB"))
		{
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_OBBCollider", "Com_OBB", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		ImGui::EndPopup();
	}

	if (ImGui::Button("RemoveParent"))
	{
		g_pObjFocused->RemoveParent();
	}

	ImGui::Separator();
	DrawTransform();

	ImGui::Separator();
	DrawModel();

	/*ImGui::Separator();
	DrawEffectImage();*/

	ImGui::Separator();
	DrawEffectSetting();

	ImGui::Separator();
	DrawBasicCollider();

}

void CInspector::UpdateUI()
{
	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");
	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("Image"))
		{
			/* Add Collider */
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_VIBuffer_RectUI", "Com_VIBuffer", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}
		if (ImGui::MenuItem("Text"))
		{
			/* Add Collider */
			if (FAILED(g_pObjFocused->AddComponent(0, "Prototype_Text", "Com_Text", g_pObjFocused->GetComponent("Com_Transform"))))
				MSG_BOX("Failed to AddComponent");
		}

		ImGui::EndPopup();
	}
	if (ImGui::Button("RemoveParent")) {
		g_pObjFocused->RemoveParent();
	}

	DrawRectTransform();

	DrawImage();

	DrawTextUI();
}


void CInspector::DrawVec3(const string & label, _float3 & values)
{
	// Make each vec3 control unique
	ImGui::PushID(label.c_str());

	float columnWitdh = 100.f;
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWitdh);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;
	ImVec2 buttonSize = { lineHeight + 3.f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.f });
	if (ImGui::Button("X", buttonSize))
		values.x = (int)values.x;
	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.1f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.8f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.7f, 0.1f, 1.f });
	if (ImGui::Button("Y", buttonSize))
		values.y = (int)values.y;
	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.8f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.8f, 1.f });
	if (ImGui::Button("Z", buttonSize))
		values.z = (int)values.z;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PopStyleVar();
	ImGui::Columns(1);

	ImGui::PopID();
}

void CInspector::DrawRectDesc(const string & label, _float & x, _float & y)
{
	ImGui::PushID(label.c_str());

	float columnWitdh = 100.f;
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWitdh);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;
	ImVec2 buttonSize = { lineHeight + 3.f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.f });
	if (ImGui::Button("X", buttonSize))
		x = (int)x;
	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##X", &x, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.1f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.8f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.7f, 0.1f, 1.f });
	if (ImGui::Button("Y", buttonSize))
		y = (int)y;
	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##Y", &y, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PopStyleVar();
	ImGui::Columns(1);

	ImGui::PopID();
}

void CInspector::DrawImage()
{
	CComponent* pComponent;
	if (pComponent = g_pObjFocused->GetComponent("Com_VIBuffer"))
	{
		ImGui::Separator();

		ImGui::Text("Shrink");
		ImGui::SameLine();
		_bool& shrink = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetIsShrink();
		ImGui::Checkbox("##isShrink", &shrink);
		dynamic_cast<CEmptyUI*>(g_pObjFocused)->SetIsShrink(shrink);
		if (shrink) {
			ImGui::Text("X");
			ImGui::SameLine();
			_bool& xShrink = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetXShrink();
			ImGui::Checkbox("##isXShrink", &xShrink);
			dynamic_cast<CEmptyUI*>(g_pObjFocused)->SetIsXShrink(xShrink);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			_bool& yShrink = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetYShrink();
			ImGui::Checkbox("##isYShrink", &yShrink);
			dynamic_cast<CEmptyUI*>(g_pObjFocused)->SetIsYShrink(yShrink);

			ImGui::Text("Breath");
			ImGui::SameLine();
			_bool& Breath = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetIsBreath();
			ImGui::Checkbox("##isBreath", &Breath);
			dynamic_cast<CEmptyUI*>(g_pObjFocused)->SetIsBreath(Breath);

			_float& _max = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetShrinkMax();
			_float& _min = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetShrinkMin();
			_float& _spd = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetShrinkSpd();
			ImGui::DragFloat("Max", &_max, 0.05f, 0.f, 1280.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
			ImGui::Columns(2);
			ImGui::DragFloat("Max", &_max, 0.05f, 0.f, 1280.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
			ImGui::Columns(2);
			ImGui::DragFloat("Min", &_min, 0.05f, -1280.f, 1280.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("Spd", &_spd, 0.01f, -1000.f, 1000.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
			ImGui::Separator();
		}



		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("Image");
		ImGui::SameLine(ImGui::GetWindowWidth() - 10);
		if (ImGui::Button("X##RemoveComponent"))
			bDelete = true;

		if (open)
		{
			// TODO: Check if it has image source
			string imageName = dynamic_cast<CVIBuffer_RectUI*>(pComponent)->GetTextureFilePath(CVIBuffer_RectUI::TEXTURE_DIFFUSE).c_str();
			imageName = imageName == "" ? "None" : imageName;
			ImGui::Text("Diffuse:");
			ImGui::Text(imageName.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//string s = *(static_cast<std::string *>(payload->Data));
					//const int s = *(const int*)(payload->Data);
					const char* s = (const char*)(payload->Data);
					//string sdf = "ss" + to_string(s);
					dynamic_cast<CVIBuffer_RectUI*>(pComponent)->UpdateTexture(s, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
				}
				ImGui::EndDragDropTarget();
			}

			imageName = dynamic_cast<CVIBuffer_RectUI*>(pComponent)->GetTextureFilePath(CVIBuffer_RectUI::TEXUTRE_MASK).c_str();
			imageName = imageName == "" ? "None" : imageName;
			ImGui::Text("Mask:");
			ImGui::Text(imageName.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//string s = *(static_cast<std::string *>(payload->Data));
					//const int s = *(const int*)(payload->Data);
					const char* s = (const char*)(payload->Data);
					//string sdf = "ss" + to_string(s);
					dynamic_cast<CVIBuffer_RectUI*>(pComponent)->UpdateTexture(s, CVIBuffer_RectUI::TEXUTRE_MASK);
				}
				ImGui::EndDragDropTarget();
			}

			imageName = dynamic_cast<CVIBuffer_RectUI*>(pComponent)->GetTextureFilePath(CVIBuffer_RectUI::TEXUTRE_NOISE).c_str();
			imageName = imageName == "" ? "None" : imageName;
			ImGui::Text("Noise:");
			ImGui::Text(imageName.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//string s = *(static_cast<std::string *>(payload->Data));
					//const int s = *(const int*)(payload->Data);
					const char* s = (const char*)(payload->Data);
					//string sdf = "ss" + to_string(s);
					dynamic_cast<CVIBuffer_RectUI*>(pComponent)->UpdateTexture(s, CVIBuffer_RectUI::TEXUTRE_NOISE);
				}
				ImGui::EndDragDropTarget();
			}

			imageName = dynamic_cast<CVIBuffer_RectUI*>(pComponent)->GetTextureFilePath(CVIBuffer_RectUI::TEXUTRE_NORMAL).c_str();
			imageName = imageName == "" ? "None" : imageName;
			ImGui::Text("Any:");
			ImGui::Text(imageName.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//string s = *(static_cast<std::string *>(payload->Data));
					//const int s = *(const int*)(payload->Data);
					const char* s = (const char*)(payload->Data);
					//string sdf = "ss" + to_string(s);
					dynamic_cast<CVIBuffer_RectUI*>(pComponent)->UpdateTexture(s, CVIBuffer_RectUI::TEXUTRE_NORMAL);
				}
				ImGui::EndDragDropTarget();
			}

			_float4& color = dynamic_cast<CVIBuffer_RectUI*>(pComponent)->GetColor();
			ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float);
			ImGui::TreePop();
		}

		if (bDelete)
			g_pObjFocused->RemoveComponent("Com_VIBuffer");
	}
}

void CInspector::DrawEffectImage()
{
	CComponent* pComponent;

	if (pComponent = g_pObjFocused->GetComponent("Com_VIBuffer"))
	{
		ImGui::Separator();

		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("TextureEffect");
		ImGui::SameLine(ImGui::GetWindowWidth() - 10);
		if (ImGui::Button("X##RemoveComponent"))
			bDelete = true;

		if (open)
		{
			// TODO: Check if it has image source
			string TextureFilePath = dynamic_cast<CVIBuffer_RectEffect*>(pComponent)->GetTextureFilePath().c_str();

			if (TextureFilePath == "")
				TextureFilePath = "None";
			else
			{
				char	szFileName[MAX_PATH] = "";

				_splitpath(TextureFilePath.c_str(), nullptr, nullptr, szFileName, nullptr);
				TextureFilePath = szFileName;
			}

			ImGui::Text("Diffuse :");
			ImGui::SameLine();
			ImGui::Text(TextureFilePath.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					const char* FilePath = (const char*)(payload->Data);

					dynamic_cast<CVIBuffer_RectEffect*>(pComponent)->SetTexture(FilePath);
				}
				ImGui::EndDragDropTarget();
			}

			/*_float4& color = dynamic_cast<CVIBuffer_RectUI*>(pComponent)->GetColor();
			ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float);*/
			ImGui::TreePop();
		}

		if (bDelete)
			g_pObjFocused->RemoveComponent("Com_VIBuffer");
	}
}

void CInspector::DrawEffectSetting()
{
	CComponent* pComponent = nullptr;
	pComponent = g_pObjFocused->GetComponent("Com_VIBuffer");
	if (pComponent == nullptr)
		pComponent = g_pObjFocused->GetComponent("Com_Model");
	if (pComponent == nullptr)
		pComponent = g_pObjFocused->GetComponent("Com_PointInstance");

	if (pComponent)
	{
		ImGui::Separator();

		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("Setting");
		/*ImGui::SameLine(ImGui::GetWindowWidth() - 10);
		if (ImGui::Button("X##RemoveComponent"))
		bDelete = true;*/

		if (open)
		{
			// TODO: Check if it has image source

			// Setup Diffuse Texture
			string TextureFilePath = static_cast<CEmptyEffect*>(g_pObjFocused)->GetTextureFilePath(CEmptyEffect::TEXTURE_DIFFUSE).c_str();

			if (TextureFilePath == "")
				TextureFilePath = "None";
			else
			{
				char	szFileName[MAX_PATH] = "";

				_splitpath(TextureFilePath.c_str(), nullptr, nullptr, szFileName, nullptr);
				TextureFilePath = szFileName;
			}

			ImGui::Text("Diffuse :");
			ImGui::SameLine();
			ImGui::Text(TextureFilePath.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					const char* FilePath = (const char*)(payload->Data);

					static_cast<CEmptyEffect*>(g_pObjFocused)->SetTexture(FilePath, CEmptyEffect::TEXTURE_DIFFUSE);
				}
				ImGui::EndDragDropTarget();
			}

			// Setup Mask Texture
			TextureFilePath = static_cast<CEmptyEffect*>(g_pObjFocused)->GetTextureFilePath(CEmptyEffect::TEXTURE_MASK).c_str();

			if (TextureFilePath == "")
				TextureFilePath = "None";
			else
			{
				char	szFileName[MAX_PATH] = "";

				_splitpath(TextureFilePath.c_str(), nullptr, nullptr, szFileName, nullptr);
				TextureFilePath = szFileName;
			}

			ImGui::Text("Mask :");
			ImGui::SameLine();
			ImGui::Text(TextureFilePath.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					const char* FilePath = (const char*)(payload->Data);

					static_cast<CEmptyEffect*>(g_pObjFocused)->SetTexture(FilePath, CEmptyEffect::TEXTURE_MASK);
				}
				ImGui::EndDragDropTarget();
			}

			// Setup Noise Texture
			TextureFilePath = static_cast<CEmptyEffect*>(g_pObjFocused)->GetTextureFilePath(CEmptyEffect::TEXTURE_NOISE).c_str();

			if (TextureFilePath == "")
				TextureFilePath = "None";
			else
			{
				char	szFileName[MAX_PATH] = "";

				_splitpath(TextureFilePath.c_str(), nullptr, nullptr, szFileName, nullptr);
				TextureFilePath = szFileName;
			}

			ImGui::Text("Noise :");
			ImGui::SameLine();
			ImGui::Text(TextureFilePath.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					const char* FilePath = (const char*)(payload->Data);

					static_cast<CEmptyEffect*>(g_pObjFocused)->SetTexture(FilePath, CEmptyEffect::TEXTURE_NOISE);
				}

				ImGui::EndDragDropTarget();
			}
			/*Sprite*/
			_int iNumX = static_cast<CEmptyEffect*>(g_pObjFocused)->GetSpriteX();
			_int iNumY = static_cast<CEmptyEffect*>(g_pObjFocused)->GetSpriteY();
			_int iNumTotal = static_cast<CEmptyEffect*>(g_pObjFocused)->GetSpriteTotal();
			_float fSpriteSpeed = static_cast<CEmptyEffect*>(g_pObjFocused)->GetSpriteSpeed();
			ImGui::DragInt("SpriteNumX", &iNumX, 1.f, 0, 1);
			ImGui::DragInt("SpriteNumY", &iNumY, 1.f, 0, 1);
			ImGui::DragInt("SpriteTotal", &iNumTotal, 1.f, 0, 1);
			ImGui::DragFloat("SpriteSpeed", &fSpriteSpeed, 0.1f, 0.f, 1.f);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetSpriteX(iNumX);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetSpriteY(iNumY);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetSpriteTotal(iNumX * iNumY);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetSpriteSpeed(fSpriteSpeed);

			/* UV Animation Setting */
			_float3 vScrollSpeedX = static_cast<CEmptyEffect*>(g_pObjFocused)->GetScrollSpeedX();
			_float3 vScrollSpeedY = static_cast<CEmptyEffect*>(g_pObjFocused)->GetScrollSpeedY();
			_float fPadding = static_cast<CEmptyEffect*>(g_pObjFocused)->GetPadding();
			_float2 vDistortion1 = static_cast<CEmptyEffect*>(g_pObjFocused)->GetDistortion(0);
			_float2 vDistortion2 = static_cast<CEmptyEffect*>(g_pObjFocused)->GetDistortion(1);
			_float2 vDistortion3 = static_cast<CEmptyEffect*>(g_pObjFocused)->GetDistortion(2);
			_float fDistortionScale = static_cast<CEmptyEffect*>(g_pObjFocused)->GetDistortionScale();
			_float fDistortionBias = static_cast<CEmptyEffect*>(g_pObjFocused)->GetDistortionBias();

			DrawVec3("ScrollSpeed_X", vScrollSpeedX);
			DrawVec3("ScrollSpeed_Y", vScrollSpeedY);
			ImGui::DragFloat("Padding", &fPadding, 0.1f, 0.f, 1.f);
			DrawRectDesc("Distortion1", vDistortion1.x, vDistortion1.y);
			DrawRectDesc("Distortion2", vDistortion2.x, vDistortion2.y);
			DrawRectDesc("Distortion3", vDistortion3.x, vDistortion3.y);

			ImGui::DragFloat("DistortionScale", &fDistortionScale, 0.1f, 0.f, 10.f);
			ImGui::DragFloat("DistortionBias", &fDistortionBias, 0.1f, 0.f, 10.f);
						
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetScrollSpeedX(vScrollSpeedX);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetScrollSpeedY(vScrollSpeedY);
			static_cast<CEmptyEffect*>(g_pObjFocused)->setDistortion(0, vDistortion1);
			static_cast<CEmptyEffect*>(g_pObjFocused)->setDistortion(1, vDistortion2);
			static_cast<CEmptyEffect*>(g_pObjFocused)->setDistortion(2, vDistortion3);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetDistortionScale(fDistortionScale);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetDistortionBias(fDistortionBias);

			/* Effect Setting */
			_float fEffectDuration = static_cast<CEmptyEffect*>(g_pObjFocused)->GetEffectDuration();

			_bool bFadeIn = static_cast<CEmptyEffect*>(g_pObjFocused)->GetFadeInEnable();
			_float fFadeInDuration = static_cast<CEmptyEffect*>(g_pObjFocused)->GetFadeInDuration();
			_float fFadeInStartTIme = static_cast<CEmptyEffect*>(g_pObjFocused)->GetFadeInStartTime();
			_bool bFadeOut = static_cast<CEmptyEffect*>(g_pObjFocused)->GetFadeOutEnable();
			_float fFadeOutDuration = static_cast<CEmptyEffect*>(g_pObjFocused)->GetFadeOutDuration();
			_float fFadeOutStartTIme = static_cast<CEmptyEffect*>(g_pObjFocused)->GetFadeOutStartTime();

			ImGui::Checkbox("Fade In", &bFadeIn);
			if (bFadeIn)
			{
				ImGui::DragFloat("FadeInStartTIme", &fFadeInStartTIme, 0.1f, 0.f, 10.f);
				ImGui::DragFloat("FadeInDuration", &fFadeInDuration, 0.1f, 0.f, 10.f);

				static_cast<CEmptyEffect*>(g_pObjFocused)->SetFadeInDuration(fFadeInDuration);
				static_cast<CEmptyEffect*>(g_pObjFocused)->SetFadeInStartTime(fFadeInStartTIme);
			}

			ImGui::Checkbox("Fade Out", &bFadeOut);
			if (bFadeOut)
			{
				ImGui::DragFloat("FadeOutStartTIme", &fFadeOutStartTIme, 0.1f, 0.f, 10.f);
				ImGui::DragFloat("FadeOutDuration", &fFadeOutDuration, 0.1f, 0.f, 10.f);

				static_cast<CEmptyEffect*>(g_pObjFocused)->SetFadeOutDuration(fFadeOutDuration);
				static_cast<CEmptyEffect*>(g_pObjFocused)->SetFadeOutStartTime(fFadeOutStartTIme);
			}

			ImGui::DragFloat("EffectDuration", &fEffectDuration, 0.1f, 0.f, 10.f);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetEffectDuration(fEffectDuration);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetFadeInEnable(bFadeIn);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetFadeOutEnable(bFadeOut);

			ImGui::TreePop();
		}

		if (bDelete)
			g_pObjFocused->RemoveComponent("Com_VIBuffer");
	}
}

void CInspector::DrawTextUI()
{
	CComponent* pComponent;
	if (pComponent = g_pObjFocused->GetComponent("Com_Text"))
	{
		CText* pText = dynamic_cast<CText*>(pComponent);
		ImGui::Separator();


		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("Text");
		ImGui::SameLine(ImGui::GetWindowWidth() - 10);
		if (ImGui::Button("X##RemoveComponent"))
			bDelete = true;

		if (open)
		{
			ImGui::Text("isCenter");
			_bool& isCenter = pText->GetIsCenter();
			ImGui::Checkbox("isCenter", &isCenter);
			pText->SetIsCenter(isCenter);
			ImGui::InputTextMultiline("Text", &pText->GetText());

			_float2& scale = pText->GetScale();
			DrawRectDesc("Scale", scale.x, scale.y);

			_float4& color = pText->GetColor();
			ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float);
			ImGui::TreePop();
		}


		if (bDelete)
			g_pObjFocused->RemoveComponent("Com_Text");
	}
}


void CInspector::DrawCollider()
{
	CComponent* pComponent;
	if (pComponent = g_pObjFocused->GetComponent("Com_Collider"))
	{
		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("Collider");
		ImGui::SameLine(ImGui::GetWindowWidth() - 10);
		if (ImGui::Button("X##RemoveComponent"))
			bDelete = true;

		if (open)
		{
			_float3& center = dynamic_cast<CCollider*>(pComponent)->GetRelativePos();
			DrawVec3("Center", center);

			if (dynamic_cast<CSphereCollider*>(pComponent))
			{
				float radius = dynamic_cast<CSphereCollider*>(pComponent)->GetSize();
				ImGui::DragFloat("Radius", &radius, 0.01f, 0.01f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
				dynamic_cast<CSphereCollider*>(pComponent)->SetSize(radius);
			}
			else if (dynamic_cast<CBoxCollider*>(pComponent))
			{
				_float3 size = dynamic_cast<CBoxCollider*>(pComponent)->GetSize();
				ImGui::DragFloat("X", &size.x, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
				ImGui::DragFloat("Y", &size.y, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
				ImGui::DragFloat("Z", &size.z, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
				dynamic_cast<CBoxCollider*>(pComponent)->SetSize(size);
			}
			else if (dynamic_cast<CCapsuleCollider*>(pComponent))
			{
				pair<float, float> size = dynamic_cast<CCapsuleCollider*>(pComponent)->GetSize();
				ImGui::DragFloat("X", &size.first, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
				ImGui::DragFloat("Y", &size.second, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
				dynamic_cast<CCapsuleCollider*>(pComponent)->SetSize(size);
			}

			_float3 angle = dynamic_cast<CCollider*>(pComponent)->GetEuler();
			DrawVec3("angle", angle);
			dynamic_cast<CCollider*>(pComponent)->SetEuler(angle);

			CCollider* pCollider = dynamic_cast<CCollider*>(pComponent);
			ImGui::Checkbox("RigidBody", &pCollider->GetRigidBodyDesc().bEnabled);
			if (pCollider->GetRigidBodyDesc().bEnabled)
			{
				ImGui::Indent();

				CCollider::RIGIDBODYDESC desc = pCollider->GetRigidBodyDesc();
				ImGui::Checkbox("Use Gravity", &pCollider->GetRigidBodyDesc().bGravity);
				ImGui::Checkbox("Is Kinematic", &pCollider->GetRigidBodyDesc().bKinematic);
				ImGui::Checkbox("Character Controller", &pCollider->GetRigidBodyDesc().bCC);

				ImGui::Unindent();
			}



			ImGui::TreePop();
		}

		if (bDelete)
			g_pObjFocused->RemoveComponent("Com_Collider");
	}
}

void CInspector::DrawTransform()
{
	CComponent* pObjTransform = nullptr;
	if (g_pObjFocused->GetParent() != nullptr) {
		if (!(pObjTransform = g_pObjFocused->GetComponent("Com_LocalTransform")))
			MSG_BOX("Failed to Get LocalTransform");
	}
	/*else if (dynamic_cast<CEmptyEffect*>(g_pObjFocused))
	{
		if (static_cast<CEmptyEffect*>(g_pObjFocused)->CheckLinkTarget())
			pObjTransform = g_pObjFocused->GetComponent("Com_LocalTransform");
		else
			pObjTransform = g_pObjFocused->GetComponent("Com_Transform");
	}*/
	else {
		if (!(pObjTransform = g_pObjFocused->GetComponent("Com_Transform")))
			MSG_BOX("Failed to Get Transform");
	}
	float _objMat[16];
	XMFLOAT4X4 objMat = dynamic_cast<CTransform*>(pObjTransform)->GetMatrix();
	memcpy(_objMat, &objMat, sizeof(XMFLOAT4X4));

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGuizmo::DecomposeMatrixToComponents(_objMat, matrixTranslation, matrixRotation, matrixScale);
	_float3 tr, rt, sc;
	memcpy(&tr, matrixTranslation, sizeof(float) * 3);
	memcpy(&rt, matrixRotation, sizeof(float) * 3);
	memcpy(&sc, matrixScale, sizeof(float) * 3);

	if (ImGui::TreeNodeEx("Transform"))
	{
		DrawVec3("Transform", tr);
		DrawVec3("Rotation", rt);
		DrawVec3("Scale", sc);

		if (dynamic_cast<CEmptyGameObject*>(g_pObjFocused))
		{
			_int& renderIdx = static_cast<CEmptyGameObject*>(g_pObjFocused)->GetRenderGroup();
			ImGui::DragInt("Render Group", &(_int)renderIdx, 1.f, 0, 2);
		}
		_float& frustumRange = g_pObjFocused->GetFrustum();
		ImGui::DragFloat("frustumRange", &frustumRange, 0.1f, 0, 1000000.f);
		_int& render = g_pObjFocused->GetPassIndex();
		ImGui::DragInt("PassIndex", &render, 0.1f, 0, 10);

		if (dynamic_cast<CEmptyEffect*>(g_pObjFocused))
		{
			_bool billBord = static_cast<CEmptyEffect*>(g_pObjFocused)->GetBillBord();
			ImGui::Checkbox("BillBord", &billBord);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetBillBord(billBord);

			ImGui::SameLine();
			_bool bTargetLink = static_cast<CEmptyEffect*>(g_pObjFocused)->CheckLinkTarget();
			ImGui::Checkbox("TargetLink", &bTargetLink);
			static_cast<CEmptyEffect*>(g_pObjFocused)->SetLinkTarget(bTargetLink);

			if (static_cast<CEmptyEffect*>(g_pObjFocused)->CheckLinkTarget())
			{
				char szTargetObjName[64];
				char szTargetBoneName[64];

				sprintf_s(szTargetObjName, static_cast<CEmptyEffect*>(g_pObjFocused)->GetTargetObjectName().c_str());
				ImGui::InputText("TargetObjName", szTargetObjName, IM_ARRAYSIZE(szTargetObjName));
				sprintf_s(szTargetBoneName, static_cast<CEmptyEffect*>(g_pObjFocused)->GetTargetBoneName().c_str());
				ImGui::InputText("TargetBoneName", szTargetBoneName, IM_ARRAYSIZE(szTargetBoneName));

				static_cast<CEmptyEffect*>(g_pObjFocused)->SetTargetObjectName(string(szTargetObjName));
				static_cast<CEmptyEffect*>(g_pObjFocused)->SetTargetBoneName(string(szTargetBoneName));

				ImGui::Text("OffSetTransform");
				CTransform* OffsetTransform = static_cast<CTransform*>(g_pObjFocused->GetComponent("Com_LocalTransform"));
				float Localmatrix[16];
				_float4x4 OffsetMatrix = dynamic_cast<CTransform*>(OffsetTransform)->GetMatrix();
				memcpy(Localmatrix, &OffsetMatrix, sizeof(_float4x4));
				float OffsetTranslation[3], OffsetRotation[3], OffsetScale[3];
				ImGuizmo::DecomposeMatrixToComponents(Localmatrix, OffsetTranslation, OffsetRotation, OffsetScale);
				_float3 Trans, Rotate, Scale;
				memcpy(&Trans, OffsetTranslation, sizeof(float) * 3);
				memcpy(&Rotate, OffsetRotation, sizeof(float) * 3);
				memcpy(&Scale, OffsetScale, sizeof(float) * 3);

				DrawVec3("Transform", Trans);
				DrawVec3("Rotation", Rotate);
				DrawVec3("Scale", Scale);

				memcpy(OffsetTranslation, &Trans, sizeof(float) * 3);
				memcpy(OffsetRotation, &Rotate, sizeof(float) * 3);
				memcpy(OffsetScale, &Scale, sizeof(float) * 3);

				ImGuizmo::RecomposeMatrixFromComponents(
					OffsetTranslation,
					OffsetRotation,
					OffsetScale, Localmatrix);

				memcpy(&OffsetMatrix, Localmatrix, sizeof(_float4x4));
				OffsetTransform->SetMatrix(OffsetMatrix);

				if (ImGui::Button("SetTarget"))
				{					
					static_cast<CEmptyEffect*>(g_pObjFocused)->LinkTransform();
				}
			}
			else
				static_cast<CEmptyEffect*>(g_pObjFocused)->RemoveTargetObject();
		}
		ImGui::TreePop();
	}

	

	memcpy(matrixTranslation, &tr, sizeof(float) * 3);
	memcpy(matrixRotation, &rt, sizeof(float) * 3);
	memcpy(matrixScale, &sc, sizeof(float) * 3);
	memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

	ImGuizmo::RecomposeMatrixFromComponents(
		matrixTranslation,
		matrixRotation,
		matrixScale, _objMat);



	m_pGizmo->SetObjMat(_objMat);
}

void CInspector::DrawInstansModelTransform()
{
	CComponent* pObjTransform = nullptr;
	if (g_pObjFocused->GetParent() != nullptr) {
		if (!(pObjTransform = g_pObjFocused->GetComponent("Com_LocalTransform")))
			MSG_BOX("Failed to Get Transform");
	}
	else {
		if (!(pObjTransform = g_pObjFocused->GetComponent("Com_Transform")))
			MSG_BOX("Failed to Get Transform");
	}
	float _objMat[16];
	XMFLOAT4X4 objMat = dynamic_cast<CTransform*>(pObjTransform)->GetMatrix();
	memcpy(_objMat, &objMat, sizeof(XMFLOAT4X4));

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGuizmo::DecomposeMatrixToComponents(_objMat, matrixTranslation, matrixRotation, matrixScale);
	_float3 tr, rt, sc;
	memcpy(&tr, matrixTranslation, sizeof(float) * 3);
	memcpy(&rt, matrixRotation, sizeof(float) * 3);
	memcpy(&sc, matrixScale, sizeof(float) * 3);

	if (ImGui::TreeNodeEx("Transform"))
	{
		DrawVec3("Transform", tr);
		DrawVec3("Rotation", rt);
		DrawVec3("Scale", sc);

		if (dynamic_cast<CEmptyMapObject*>(g_pObjFocused))
		{
			_int& renderIdx = dynamic_cast<CEmptyMapObject*>(g_pObjFocused)->GetRenderGroup();
			ImGui::DragInt("Render Group", &(_int)renderIdx, 1.f, 0, 2);
		}
		_float& frustumRange = g_pObjFocused->GetFrustum();
		ImGui::DragFloat("frustumRange", &frustumRange, 0.1f, 0, 50);
		_int& render = g_pObjFocused->GetPassIndex();
		ImGui::DragInt("PassIndex", &render, 0.1f, 0, 10);
		ImGui::TreePop();
	}


	memcpy(matrixTranslation, &tr, sizeof(float) * 3);
	memcpy(matrixRotation, &rt, sizeof(float) * 3);
	memcpy(matrixScale, &sc, sizeof(float) * 3);
	memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

	ImGuizmo::RecomposeMatrixFromComponents(
		matrixTranslation,
		matrixRotation,
		matrixScale, _objMat);



	m_pGizmo->SetObjMat(_objMat);
}


void CInspector::DrawBuffer()
{
	CComponent* pVIBuffer = nullptr;
	if (pVIBuffer = g_pObjFocused->GetComponent("Com_VIBuffer"))
	{
		bool bDelete = false;

		if (dynamic_cast<CVIBuffer_Terrain*>(pVIBuffer))
		{
			bool open = ImGui::TreeNodeEx("Terrain");
			ImGui::SameLine(ImGui::GetWindowWidth() - 10);
			if (ImGui::Button("X##RemoveComponent"))
				bDelete = true;
			if (open)
			{
				string heightMapPath = dynamic_cast<CVIBuffer_Terrain*>(pVIBuffer)->GetHeightMapPath().c_str();
				heightMapPath = heightMapPath == "" ? "None" : heightMapPath;
				ImGui::Text(heightMapPath.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
					{
						const char* s = (const char*)(payload->Data);
						dynamic_cast<CVIBuffer_Terrain*>(pVIBuffer)->SetHeightMapPath(s);
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::TreePop();
			}
		}
		else {
			bool open = ImGui::TreeNodeEx("VIBuffer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 10);
			if (ImGui::Button("X##RemoveComponent"))
				bDelete = true;
			if (open)
			{
				string texturePath = dynamic_cast<CVIBuffer_Rect*>(pVIBuffer)->GetTextureFilePath();
				texturePath = texturePath == "" ? "None" : texturePath;
				ImGui::Text(texturePath.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
					{
						const char* s = (const char*)(payload->Data);
						dynamic_cast<CVIBuffer_Rect*>(pVIBuffer)->UpdateTexture(s);
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::TreePop();
			}
		}

		if (bDelete)
			g_pObjFocused->RemoveComponent("Com_VIBuffer");
	}
}

void CInspector::DrawModel()
{
	CComponent* pModel = nullptr;

	/* DrawModel GameObject */
	if (dynamic_cast<CEmptyGameObject*>(g_pObjFocused))
	{
		if (pModel = g_pObjFocused->GetComponent("Com_Model"))
		{
			bool bDelete = false;

			if (dynamic_cast<CModel*>(pModel))
			{
				bool open = ImGui::TreeNodeEx("Model");
				ImGui::SameLine(ImGui::GetWindowWidth() - 10);
				if (ImGui::Button("X##RemoveComponent"))
					bDelete = true;

				if (open)
				{
					_bool& hasCollider = dynamic_cast<CModel*>(pModel)->HasMeshCollider();
					ImGui::Checkbox("Mesh Collider", &hasCollider);

					_bool& Equipment = dynamic_cast<CModel*>(pModel)->GetLinkEquip();
					ImGui::Checkbox("Equipment", &Equipment);
					dynamic_cast<CModel*>(pModel)->SetLinkEquip(Equipment);

					ImGui::Separator();

					if (pModel != nullptr) {
						_bool hasAnimation = dynamic_cast<CModel*>(pModel)->Get_IsAnimationMesh();
						if (hasAnimation)
						{
							static int layer_current_idx = 0;
							ImGui::Text("BoneList:");
							if (ImGui::BeginListBox("BonesName", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
							{
								unordered_map<string, BONEDESC*> bones = dynamic_cast<CModel*>(pModel)->GetBoneDescMap();
								vector<string> boneNames;
								for (auto iter : bones)
								{
									boneNames.emplace_back(iter.first);
								}
								for (int n = 0; n < boneNames.size(); n++)
								{
									const bool is_selected = (layer_current_idx == n);
									if (ImGui::Selectable(boneNames[n].c_str(), is_selected))
									{
										layer_current_idx = n;
										string name = boneNames[layer_current_idx];
									}
									if (is_selected)
									{
										ImGui::SetItemDefaultFocus();
									}

								}
								ImGui::EndListBox();
							}
						}

					}
					ImGui::Separator();

					string meshFileName = dynamic_cast<CModel*>(pModel)->GetMeshFileName().c_str();
					meshFileName = meshFileName == "" ? "None" : meshFileName;
					ImGui::Text(meshFileName.c_str());
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
						{
							const char* s = (const char*)(payload->Data);

							char	szTextureFileName[MAX_PATH] = "";
							char	szExt[MAX_PATH] = "";
							char	szDir[MAX_PATH] = "";
							_splitpath(s, nullptr, szDir, szTextureFileName, szExt);
							
							if (!strcmp(szExt, ".fbx") || !strcmp(szExt, ".Fbx") || !strcmp(szExt, ".FBX"))
							{
								strcat_s(szTextureFileName, szExt);
								g_pObjFocused->RemoveComponent("Com_Model");
								CEngine::GetInstance()->CloneModel(g_pObjFocused, szDir, szTextureFileName, "", "", false, g_pObjFocused->GetComponent("Com_Transform"));
							}
						}

						ImGui::EndDragDropTarget();
					}

					
					ImGui::TreePop();
				}

			}

			if (bDelete)
				g_pObjFocused->RemoveComponent("Com_Model");
		}
	}

	/* DrawModel Effect */
	else
	{
		if (pModel = g_pObjFocused->GetComponent("Com_Model"))
		{
			bool bDelete = false;

			if (dynamic_cast<CModel*>(pModel))
			{
				bool open = ImGui::TreeNodeEx("MeshEffect");
				ImGui::SameLine(ImGui::GetWindowWidth() - 10);
				if (ImGui::Button("X##RemoveComponent"))
					bDelete = true;

				if (open)
				{
					/*_bool& hasCollider = dynamic_cast<CModel*>(pModel)->HasMeshCollider();
					ImGui::Checkbox("Mesh Collider", &hasCollider);*/

					ImGui::Text("Mesh :");
					ImGui::SameLine();
					string meshFileName = dynamic_cast<CModel*>(pModel)->GetMeshFileName().c_str();
					meshFileName = meshFileName == "" ? "None" : meshFileName;
					/* DragDrop Mesh */
					ImGui::Text(meshFileName.c_str());
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
						{
							const char* s = (const char*)(payload->Data);

							char	szTextureFileName[MAX_PATH] = "";
							char	szExt[MAX_PATH] = "";
							char	szDir[MAX_PATH] = "";
							_splitpath(s, nullptr, szDir, szTextureFileName, szExt);

							if (!strcmp(szExt, ".fbx") || !strcmp(szExt, ".Fbx") || !strcmp(szExt, ".FBX"))
							{
								strcat_s(szTextureFileName, szExt);
								g_pObjFocused->RemoveComponent("Com_Model");
								CEngine::GetInstance()->CloneModel(g_pObjFocused, szDir, szTextureFileName, "../../Assets/Shader/Shader_Effect.fx", "", false, g_pObjFocused->GetComponent("Com_Transform"));
							}
						}
						ImGui::EndDragDropTarget();

						ImGui::TreePop();

						return;
					}

					/* DragDrop Material */
					ImGui::Text("Diffuse :");
					ImGui::SameLine();

					string EffectFileName = dynamic_cast<CModel*>(pModel)->GetEffectFilePath().c_str();
					EffectFileName = EffectFileName == "" ? "None" : EffectFileName;
					ImGui::Text(EffectFileName.c_str());
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
						{
							const char* pTextureFilePath = (const char*)(payload->Data);

							static_cast<CModel*>(pModel)->SetEffectFilePath(pTextureFilePath);
							static_cast<CModel*>(pModel)->SetEffectMaterial();
						}
						ImGui::EndDragDropTarget();
					}

					ImGui::TreePop();
				}
			}

			if (bDelete)
				g_pObjFocused->RemoveComponent("Com_Model");
		}
	}
}

void CInspector::DrawLight()
{
	CComponent* pLight = nullptr;
	if (pLight = g_pObjFocused->GetComponent("Com_Light"))
	{
		bool bDelete = false;

		if (dynamic_cast<CLight*>(pLight))
		{
			bool open = ImGui::TreeNodeEx("Light");
			ImGui::SameLine(ImGui::GetWindowWidth() - 10);
			if (ImGui::Button("X##RemoveComponent"))
				bDelete = true;

			if (open)
			{
				LIGHTDESC& lightDesc = dynamic_cast<CLight*>(pLight)->GetDesc();
				const char* items[] = { "Directional", "Point", "Spot" };
				static int lightType = lightDesc.eType;
				ImGui::Combo("combo", &lightType, items, IM_ARRAYSIZE(items));
				lightDesc.eType = (LIGHTDESC::TYPE)lightType;

				ImGui::DragFloat("Light Range", &lightDesc.fLightRange, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
				ImGui::DragFloat("Light Angle", &lightDesc.fLightAngle, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);

				ImGui::ColorEdit4("vDiffuse##2f", (float*)&lightDesc.vDiffuse, ImGuiColorEditFlags_Float);
				ImGui::ColorEdit4("vAmbient##2f", (float*)&lightDesc.vAmbient, ImGuiColorEditFlags_Float);
				ImGui::ColorEdit4("vSpecular##2f", (float*)&lightDesc.vSpecular, ImGuiColorEditFlags_Float);

				ImGui::TreePop();
			}
		}

		if (bDelete)
			g_pObjFocused->RemoveComponent("Com_Light");
	}
}

void CInspector::DrawNavi()
{
	CComponent* pComponent;
	if (pComponent = g_pObjFocused->GetComponent("Com_Navi"))
	{
		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("NPC_Info");
		ImGui::SameLine(ImGui::GetWindowWidth() - 10);
		if (ImGui::Button("X##RemoveComponent"))
			bDelete = true;

		if (open)
		{
			_uint npcId = static_cast<CNPCInfo*>(pComponent)->GetNPCID();
			_uint questID = static_cast<CNPCInfo*>(pComponent)->GetNPCQuestID();
			ImGui::Text("NPCID : ");
			ImGui::SameLine(2.f);
			ImGui::TreePop();
		}

		if (bDelete) //추가되고 x버튼 누르면 삭제되는거 
			g_pObjFocused->RemoveComponent("Com_Navi");
	}
}

void CInspector::DrawInstancing()
{

}

void CInspector::DrawNPCInfo()
{
	CComponent* pComponent;
	pComponent = g_pObjFocused->GetComponent("Com_NPCInfo");
	if (pComponent) {
		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("NPC_Info");
		if (open) {
			CNPCInfo* info = static_cast<CNPCInfo*>(pComponent);
			_int id = info->GetNPCID();
			_int questid = info->GetNPCQuestID();
			_int boxid = info->GetBoxID();

			ImGui::InputInt("NPC_ID", &id);
			ImGui::InputInt("Quest_ID", &questid);
			ImGui::InputInt("Quest_Box_ID", &boxid);
			info->SetNPCID(id);
			info->SetNPCQuestID(questid);
			info->SetBoxID(boxid);
			ImGui::TreePop();
		}
		if (bDelete) //추가되고 x버튼 누르면 삭제되는거 
			g_pObjFocused->RemoveComponent("Com_NPCInfo");
	}
}

void CInspector::DrawBasicCollider()
{
	_int obb = static_cast<CEmptyGameObject*>(g_pObjFocused)->GetNumOBB();
	for (int i = 0; i < obb; i++) {
		string com = "Com_OBB";
		if (i != 0)
			com += to_string(i);
		CComponent* pComponent;
		if (pComponent = g_pObjFocused->GetComponent(com))
		{
			string obb = "OBB";
			if (i != 0)
				obb += to_string(i);
			bool bDelete = false;
			bool open = ImGui::TreeNodeEx(obb.c_str());
			ImGui::SameLine(ImGui::GetWindowWidth() - 10);
			if (ImGui::Button("X##RemoveComponent"))
				bDelete = true;

			if (open)
			{
				if (dynamic_cast<CBasicCollider*>(pComponent))
				{
					_float3 size = dynamic_cast<CBasicCollider*>(pComponent)->GetSize();

					ImGui::DragFloat("Scail : X", &size.x, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
					ImGui::DragFloat("Scail : Y", &size.y, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
					ImGui::DragFloat("Scail : Z", &size.z, 0.1f, 0.1f, 100.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
					dynamic_cast<CBasicCollider*>(pComponent)->SetSize(size);
					ImGui::Separator();
					////////////////////////////////////////////////////////////////////////////

					_float3 pos = dynamic_cast<CBasicCollider*>(pComponent)->GetPos();
					ImGui::DragFloat("Pos : X", &pos.x, -10.f, 0.1f, 100.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
					ImGui::DragFloat("Pos : Y", &pos.y, -10.f, 0.1f, 100.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
					ImGui::DragFloat("Pos : Z", &pos.z, -10.f, 0.1f, 100.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
					dynamic_cast<CBasicCollider*>(pComponent)->SetPos(pos);
				}

				CModel* model =static_cast<CModel*>(g_pObjFocused->GetComponent("Com_Model"));
				if (model) {
					_bool hasAnimation = static_cast<CModel*>(model)->Get_IsAnimationMesh();
					if (hasAnimation)
					{
						static int current_idx = 0;
						static string name;
						ImGui::Text("BoneList:");
						if (ImGui::BeginListBox("BonesName", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
						{
							unordered_map<string, BONEDESC*> bones = static_cast<CModel*>(model)->GetBoneDescMap();
							vector<string> boneNames;
							for (auto iter : bones)
							{
								boneNames.emplace_back(iter.first);
							}
							for (int n = 0; n < boneNames.size(); n++)
							{
								const bool is_selected = (current_idx == n);
								if (ImGui::Selectable(boneNames[n].c_str(), is_selected))
								{
									current_idx = n;
									name = boneNames[current_idx];
								}
								if (is_selected)
								{
									ImGui::SetItemDefaultFocus();
								}

							}
							ImGui::EndListBox();
						}
						if (ImGui::Button("Set BoneName")) {
							static_cast<CBasicCollider*>(pComponent)->SetBoneName(name);
						}
						//obb offset
						_float3 offset = static_cast<CBasicCollider*>(pComponent)->GetOffset();
						DrawVec3("Offset", offset);
						static_cast<CBasicCollider*>(pComponent)->SetOffset(offset);
					}
				}

				_bool isAttach = static_cast<CBasicCollider*>(pComponent)->GetIsAttachBone();
				ImGui::Checkbox("##isAttach", &isAttach);
				static_cast<CBasicCollider*>(pComponent)->SetIsAttachBone(isAttach);
				if (isAttach) {
					string boneName = static_cast<CBasicCollider*>(pComponent)->GetBoneName();
					string imguiText = "Current Selected Bone is : " + boneName;
					ImGui::Text(imguiText.c_str());
				}

				_int curType = (_int)static_cast<CBasicCollider*>(pComponent)->GetCollisionType();
				string typeText = "TypeList : " + to_string(curType);
				ImGui::Text(typeText.c_str());
				if (ImGui::BeginListBox("TpyeName", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
				{
					for (int j = 0; j < (_int)ID::IDEND; j++) {
						const bool is_selected = (curType == j);
						if (ImGui::Selectable(idTypes[j].c_str(), is_selected))
						{
							curType = j;
						}
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					static_cast<CBasicCollider*>(pComponent)->SetCollisionType((ID)curType);
					ImGui::EndListBox();
				}
				ImGui::TreePop();
			}

			if (bDelete)
				g_pObjFocused->RemoveComponent(com);
		}
		else {
			break;
		}
	}
	
}

void CInspector::DrawStat()
{
	CComponent* pComponent;
	pComponent = g_pObjFocused->GetComponent("Com_Stat");
	if (pComponent) {
		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("Status");
		if (open) {
			CStat* info = static_cast<CStat*>(pComponent);
			CStat::STAT objStat = info->GetStatInfo();
			ImGui::Text("Hp Stat");
			ImGui::DragFloat("Hp", &objStat.hp, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("LevelHp", &objStat.levelHp, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("MaxHp", &objStat.maxHp, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);

			ImGui::Text("Hp Regen");
			ImGui::DragFloat("HpRegen", &objStat.hpRegeneration, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("LevelHpRegen", &objStat.levelRegen, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);

			ImGui::Text("Atk");
			ImGui::DragFloat("Atk", &objStat.atk, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("levelAtk", &objStat.levelAtk, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);

			ImGui::Text("Armor");
			ImGui::DragFloat("Armor", &objStat.armor, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("LevelArmor", &objStat.levelArmor, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);

			ImGui::Text("Exp");
			ImGui::DragFloat("Exp", &objStat.exp, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("MaxExp", &objStat.maxExp, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);
			ImGui::DragFloat("levelExp", &objStat.levelExp, 0.1f, 0.1f, 1000000.f, "%.1f", ImGuiSliderFlags_ClampOnInput);

			ImGui::DragInt("Level", &objStat.level);
			ImGui::DragInt("Gold", &objStat.gold);

			info->SetStatInfo(objStat);

			ImGui::TreePop();
		}
		if (bDelete) //추가되고 x버튼 누르면 삭제되는거 
			g_pObjFocused->RemoveComponent("Com_Stat");
	}
}

void CInspector::DrawShop()
{
	CShop* pComponent;
	pComponent =static_cast<CShop*>(g_pObjFocused->GetComponent("Com_Shop"));
	if (pComponent) {
		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("Shop");
		if (open) {
			_int type = (_int)pComponent->GetItemType();
			pair<_int, _int> range = pComponent->GetItemRange();
			_int rag[2] = { range.first,range.second };
			ImGui::InputInt("ItemType", &type);
			ImGui::InputInt2("ItemRnage", rag);

			pComponent->SetItemType(ITEMTYPE(type));
			pComponent->SetItemRange(rag[0], rag[1]);

			ImGui::TreePop();
		}
		if (bDelete) //추가되고 x버튼 누르면 삭제되는거 
			g_pObjFocused->RemoveComponent("Com_Shop");
	}
}

void CInspector::DrawSpawn()
{
	CSpawner* pComponent;
	pComponent = static_cast<CSpawner*>(g_pObjFocused->GetComponent("Com_Spawn"));
	if (pComponent) {
		bool bDelete = false;
		bool open = ImGui::TreeNodeEx("Spawn");
		if (open) {
			CSpawner::SPAWNER info = pComponent->GetSpawnInfo();
			//ImGui::inputte
			
			ImGui::InputText("##Name", &info.prefabName, 60);
			ImGui::InputFloat("Time", &info.spawnTime);

			
			pComponent->SetSpawnInfo(info);
			
			ImGui::TreePop();
		}
		if (bDelete) //추가되고 x버튼 누르면 삭제되는거 
			g_pObjFocused->RemoveComponent("Com_Shop");
	}
}

void CInspector::DrawRectTransform()
{
	CComponent* pObjTransform = nullptr;
	if (!(pObjTransform = g_pObjFocused->GetComponent("Com_Transform")))
		MSG_BOX("Failed to Get Transform");

	CRectTransform::RECTTRANSFORMDESC desc = dynamic_cast<CRectTransform*>(pObjTransform)->GetTransformDesc();

	ImGui::Separator();
	if (ImGui::TreeNodeEx("Rect Transform"))
	{
		DrawRectDesc("Position", desc.posX, desc.posY);
		DrawRectDesc("Size", desc.sizeX, desc.sizeY);
		DrawRectDesc("CorrectSize", desc.correctX, desc.correctY);

		_int& order = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetSortingOrder();
		ImGui::DragInt("Sortring Order", &order, 1, 0, 10);

		_int& render = g_pObjFocused->GetPassIndex();
		ImGui::DragInt("PassIndex", &render, 0.1f, 0, 15);

		ImGui::Text("Hover On?");
		ImGui::SameLine();
		_bool& hover = dynamic_cast<CEmptyUI*>(g_pObjFocused)->GetIsHovering();
		ImGui::Checkbox("##isHovering", &hover);
		dynamic_cast<CEmptyUI*>(g_pObjFocused)->SetIsHovering(hover);


		ImGui::TreePop();
	}

	// SetTransformMat(desc)
	dynamic_cast<CRectTransform*>(pObjTransform)->SetTransformMat(desc);

	// GetTransformMat
	float _objMat[16];
	XMFLOAT4X4 objMat = dynamic_cast<CRectTransform*>(pObjTransform)->GetTransformMat();
	memcpy(_objMat, &objMat, sizeof(XMFLOAT4X4));

	m_pGizmo->SetObjMat(_objMat);
}

void CInspector::Free()
{
}
