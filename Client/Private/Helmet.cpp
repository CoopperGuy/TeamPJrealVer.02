#include "stdafx.h"
#include "..\Public\Helmet.h"

USING(Client)

CHelmet::CHelmet()
{
}

CHelmet::CHelmet(CGameObject * pObj)
{
}

HRESULT CHelmet::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Add_EquipList("LeatherHelmet");
	Add_EquipList("PlateHelmet");
	Add_EquipList("JusinHelmet");

	return S_OK;
}

void CHelmet::Update(_double deltaTime)
{
}

void CHelmet::LateUpdate(_double deltaTime)
{
}

void CHelmet::Render()
{
}

CHelmet * CHelmet::Create()
{
	CHelmet*		pInstance = new CHelmet();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CHelmet");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHelmet::Free()
{
}