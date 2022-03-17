#include "stdafx.h"
#include "Navi.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Scene_Tool.h"
#include "ToolPlayer.h"
#include "Cell.h"

USING(Tool)
CNavi::CNavi()
{
	Initialize();
}

void CNavi::Initialize()
{
	m_pGizmo = dynamic_cast<CGizmo*>(CEngine::GetInstance()->GetWindow("Gizmo"));

	/*CEngine::GetInstance()->AddScriptObject(CToolPlayer::Create(nullptr));*/

	for (auto& temp : NaviMeshTemp)
	{
		ZeroMemory(&temp, sizeof(_float3));
	}

}

void CNavi::Update()
{
	ImGui::Begin("Navigation");
	ImGui::Separator();

	//_bool active = g_pObjFocused->IsActive();
	ImGui::Checkbox("Make_Navigation", &m_bDrawActive);

	if (m_bDrawActive)
	{
		DrawNavi();

		if (ImGui::Button("Delete_All"))
		{
			Delete_All();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete_Last"))
		{
			Delete_Last();
		}
	}

	if (ImGui::Button("Save"))
	{
		Navigation_Save();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		Navigation_Load();
	}

	ImGui::End();

}

void CNavi::DrawNavi()
{
	CComponent* TempComponent = nullptr;
	TempComponent = g_pObjFocused->GetComponent("Com_Navi");
	pNavigation = dynamic_cast<CNavigation*>(TempComponent);

	if (pNavigation)
	{
		if (CEngine::GetInstance()->IsKeyDown(VK_LBUTTON))
		{
			_float3 out;
			if (m_pGizmo->RayCast(out, g_hWnd, XMMatrixIdentity()))
			{
				out.y = 0.f;
				NaviMeshTemp[num] = out;

				if (num > 2)
				{
					for (int i = 0; i < num; i++)
					{
						_float x = out.x - NaviMeshTemp[i].x;
						if (-3.f <= x && 3.f >= x)
						{
							_float z = out.z - NaviMeshTemp[i].z;
							if (-3.f <= z && 3.f >= z)
							{
								NaviMeshTemp[num].x = NaviMeshTemp[i].x;
								NaviMeshTemp[num].z = NaviMeshTemp[i].z;
								break;
							}
						}
					}
					pNavigation->AddPoint(NaviMeshTemp[num]);
					num++;
				}
				else
				{
					NaviMeshTemp[num] = out;
					num++;

					pNavigation->AddPoint(out);
				}
			}
		}
	}
}

void CNavi::LateUpdate()
{
}

HRESULT CNavi::Navigation_Save()
{
	if (nullptr != pNavigation)
	{
		_ulong			dwByte = 0;
		HANDLE			hFile = CreateFile(TEXT("../../Assets/Data/Stage1.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		Cells = pNavigation->Get_Cells();

		_uint			iNumCells = (_uint)Cells.size();
		WriteFile(hFile, &iNumCells, sizeof(_uint), &dwByte, nullptr);

		for (auto& pCells : Cells)
		{
			_vector			vTemp[3];
			ZeroMemory(vTemp, sizeof(_vector));
			vTemp[0] = pCells->Get_Point(CCell::POINT_A);
			vTemp[1] = pCells->Get_Point(CCell::POINT_B);
			vTemp[2] = pCells->Get_Point(CCell::POINT_C);
			WriteFile(hFile, vTemp, sizeof(_vector) * 3, &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
	return S_OK;
}

HRESULT CNavi::Navigation_Load()
{
	if (nullptr != pNavigation)
	{
		pNavigation->Delete_All();
		Cells = pNavigation->Get_Cells();

		_ulong			dwByte = 0;
		HANDLE			hFile = CreateFile(TEXT("../../Assets/Data/Stage1.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		vector<class CCell*>::iterator iter = Cells.begin();
		for (; iter != Cells.end();)
		{
			iter = Cells.erase(iter);
			++iter;
		}

		ReadFile(hFile, &m_iNumCells, sizeof(_uint), &dwByte, nullptr);

		Cells.reserve(m_iNumCells);

		for (_uint i = 0; i < m_iNumCells; ++i)
		{
			_vector			vTemp[3];
			ZeroMemory(vTemp, sizeof(_vector));
			ReadFile(hFile, vTemp, sizeof(_vector) * 3, &dwByte, nullptr);

			for (_uint j = 0; j < 3; ++j)
			{
				_vector			vPoints[3];
				_float3			vPos;
				ZeroMemory(vPoints, sizeof(_vector));
				vPoints[j] = vTemp[j];
				XMStoreFloat3(&vPos, vPoints[j]);
				pNavigation->AddPoint(vPos);
			}
		}

		CloseHandle(hFile);

		if (FAILED(pNavigation->SetUp_NeighborCell()))
			return E_FAIL;

	}
	return S_OK;
}

HRESULT CNavi::Delete_All()
{
	if (nullptr != pNavigation)
		pNavigation->Delete_All();
	else
		MSG_BOX("pNavigation is Nullptr");
	return S_OK;
}

HRESULT CNavi::Delete_Last()
{
	if (nullptr != pNavigation)
		pNavigation->Delete_Last();
	else
		MSG_BOX("pNavigation is Nullptr");
	return S_OK;
}

void CNavi::Free()
{
}
