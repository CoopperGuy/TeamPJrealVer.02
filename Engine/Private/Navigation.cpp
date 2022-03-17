#include "EnginePCH.h"
#include "..\public\Navigation.h"
#include "Cell.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)

{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
{
	for (auto& pCell : m_Cells)
		SafeAddRef(pCell);

}

HRESULT CNavigation::InitializePrototype(const _tchar* pDataFiles)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pDataFiles, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_iNumCells, sizeof(_uint), &dwByte, nullptr);

	m_Cells.reserve(m_iNumCells);

	for (_uint i = 0; i < m_iNumCells; ++i)
	{
		_vector		vPoints[CCell::POINT_END];

		ReadFile(hFile, vPoints, sizeof(_vector) * CCell::POINT_END, &dwByte, nullptr);

		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	ZeroMemory(vPoints, sizeof(_vector));

	if (FAILED(SetUp_NeighborCell()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::InitializePrototype_Tool()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;

	return S_OK;

}

HRESULT CNavigation::Initialize(void * pArg)
{
	if (nullptr != pArg)
		m_NaviDesc = *(NAVIDESC*)pArg;

	return S_OK;
}

void CNavigation::AddPoint(_float3 point)
{
	//vPoints[num] = XMVectorSet(XMVectorGetX(point), XMVectorGetY(point), XMVectorGetZ(point),1.f);

	fTemp[num] = point;
	vPoints[num] = XMLoadFloat3(&point);
	num++;

	if (num >= 3)
	{
		if (this->CCW(fTemp[0], fTemp[1], fTemp[2]))
		{
			int i = 0;
		}

		num = 0;
		CellCount += 1;
		if (FAILED(PushBackCell()))
			return;
		ZeroMemory(vPoints, sizeof(_vector));
	}
}

HRESULT CNavigation::PushBackCell()
{
	CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.push_back(pCell);

	return S_OK;
}

_bool CNavigation::isMove(_fvector vPoint)
{
	CCell*		pNeighborCell = nullptr;

	if (true == m_Cells[m_NaviDesc.iCurrentCellIndex]->isIn(vPoint, &pNeighborCell)) //삼각형안에있니? 
	{
		return true;
	}
	else //현재 인덱스 삼각형 밖에있어 
	{
		if (nullptr != pNeighborCell) //이웃이있으면 
		{
			while (true) //루프돌면서
			{
				if (nullptr == pNeighborCell) // 이웃이 nullptr이면 fasle해라
					return false;

				if (true == pNeighborCell->isIn(vPoint, &pNeighborCell)) // 이웃 삼각형위에잇으면 
				{
					m_NaviDesc.iCurrentCellIndex = pNeighborCell->Get_Index(); // 현재 index 에 대입
					NowLine = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Line();
					break;
				}
			}
			return true;
		}
		else //이웃이없으면
		{
			NowLine = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Line();
			return false;
		}
	}

	return _bool();
}

void CNavigation::Sliding(_float3 & vPos)
{
	//CCell*		pNeighborCell = nullptr;

	//CTransform* m_pPlayerTransformCom = dynamic_cast<CTransform*>(pGameInstance->GetComponent(TEXT("Layer_Player"), TEXT("Com_Transform"), 0));
	//if (m_pPlayerTransformCom == nullptr)
	//{
	//	MSG_BOX("네비게이션 이웃없ㄴ느곳에서 널~");
	//	return;
	//}
	//_vector vPlayerPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);
	////현재 라인


	//NowLine = m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Line();
	//_vector vLine = XMLoadFloat3(&NowLine);
	//vLine = XMVectorSetY(vLine, 0.f);

	////플레이어 방향 
	//_vector vPlayerDir = XMVector3Normalize(m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK));
	//vPlayerDir = XMVectorSetY(vPlayerDir, 0.f);

	////슬라이딩 = 플레이어 방향백터와 법선백터를 내적 후 노말라이즈 해주고 *2 +vLine
	//m_Sliding = -XMVector3Dot(vPlayerDir, vLine) + vPlayerDir;
	//m_Sliding = XMVectorSetY(m_Sliding, 0.f);

	//XMStoreFloat3(&vPos, m_Sliding);
	////한번 더 체크해야함 

	//RELEASE_INSTANCE(CGameInstance);
}

void CNavigation::Delete_Last()
{
	SafeRelease(m_Cells.back());
	m_Cells.pop_back();
}

void CNavigation::Delete_All()
{
	for (auto& pCell : m_Cells)
		SafeRelease(pCell);
	m_Cells.clear();
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	int i = sizeof(m_Cells);

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render();
	}

	return S_OK;
}


#endif
_bool CNavigation::CCW(_float3 Point_1, _float3 Point_2, _float3 Point_3)
{
	_int result = _int((Point_2.x - Point_1.x) * (Point_3.z - Point_1.z) - (Point_3.x - Point_1.x) * (Point_2.z - Point_1.z));
	if (result > 0) {
		return true;
	}
	else {
		return false;
	} 
}

HRESULT CNavigation::SetUp_NeighborCell()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
		}
	}

	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pDataFiles)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype(pDataFiles)))
	{
		MSG_BOX("Failed to Creating CNavigation");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CNavigation * CNavigation::Create_ForTool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype_Tool()))
	{
		MSG_BOX("Failed to Creating CNavigation_ForTool");
		SafeRelease(pInstance);
	}

	return pInstance;
}



CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Creating CNavigation");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		SafeRelease(pCell);
	m_Cells.clear();
}
