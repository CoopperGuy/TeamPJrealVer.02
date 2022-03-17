#include "stdafx.h"
#include "MapInstancing.h"

#include "GameObject.h"
#include "Scene_Tool.h"
#include "Engine.h"
#include "InstanceModel.h"
#include "EmptyMapObject.h"

USING(Tool)
CMapInstancing::CMapInstancing()
{
	Initialize();

	/*if (dynamic_cast<CEmptyMapObject*>(g_pObjFocused))*/
}

void CMapInstancing::Initialize()
{
	m_pGizmo = dynamic_cast<CGizmo*>(CEngine::GetInstance()->GetWindow("Gizmo"));

	/*CEngine::GetInstance()->AddScriptObject(CToolPlayer::Create(nullptr));*/


}

void CMapInstancing::Update()
{
	ImGui::Begin("MapInstancing");
	ImGui::Separator();

	CComponent* pModel = nullptr;
	//pModel = g_pObjFocused->GetComponent("Com_InstanceModel");
	//_bool active = g_pObjFocused->IsActive();

	ImGui::DragInt("InsCount", &InstanceCount, 1, 1, 100);

	if (ImGui::Button("Add"))
	{
		if (dynamic_cast<CEmptyMapObject*>(g_pObjFocused))
		{
			bool bDelete = false;

			if (dynamic_cast<CInstanceModel*>(pModel))
			{
				bool open = ImGui::TreeNodeEx("Model");
				ImGui::SameLine(ImGui::GetWindowWidth() - 10);
				if (ImGui::Button("X##RemoveComponent"))
					bDelete = true;

				if (open)
				{
					/*string meshFileName = dynamic_cast<CInstanceModel*>(pModel)->GetMeshFileName().c_str();
					meshFileName = meshFileName == "" ? "None" : meshFileName;*/
					//ImGui::Text(meshFileName.c_str());
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
						{
							const char* s = (const char*)(payload->Data);

							char	szTextureFileName[MAX_PATH] = "";
							char	szExt[MAX_PATH] = ""; //È®ÀåÀÚ
							char	szDir[MAX_PATH] = "";
							_splitpath(s, nullptr, szDir, szTextureFileName, szExt);

							if (!strcmp(szExt, ".fbx") || !strcmp(szExt, ".Fbx") || !strcmp(szExt, ".FBX"))
							{
								strcat_s(szTextureFileName, szExt);

								/* For.Model Component */
								/* Component_Model_Player*/
								_matrix		ScaleMatrix, RotationMatrix, TranslationMatrix;
								_matrix		ModelPivotMatrix;
								ScaleMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
								//RotationMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f));

								ModelPivotMatrix = ScaleMatrix/* * RotationMatrix*/;

								ID3D11Device* Device = CEngine::GetInstance()->GetDevice();
								ID3D11DeviceContext* DeviceContext = CEngine::GetInstance()->GetDeviceContext();

								//if (FAILED(CEngine::GetInstance()->AddPrototype(0, "Prototype_InstanceModel", CInstanceModel::Create(Device, DeviceContext, szDir, szTextureFileName, TEXT("../../Assets/Shader/Shader_ModelInstance.fx"), ModelPivotMatrix, InstanceCount))))
								//	return;

								//g_pObjFocused = pObj;


								/*g_pObjFocused->RemoveComponent("Com_Model");
								CEngine::GetInstance()->CloneModel(g_pObjFocused, szDir, szTextureFileName, "", "", false, g_pObjFocused->GetComponent("Com_Transform"));*/
							}
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::TreePop();
				}
			}
			if (bDelete)
			{
				g_pObjFocused->RemoveComponent("Com_InsModel");
			}
		}

	}
	ImGui::End();
}


void CMapInstancing::LateUpdate()
{
}


void CMapInstancing::Free()
{
}
