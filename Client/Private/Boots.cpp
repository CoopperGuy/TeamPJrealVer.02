#include "stdafx.h"
#include "..\Public\Boots.h"

USING(Client)

CBoots::CBoots()
{
}

CBoots::CBoots(CGameObject * pObj)
{
}

HRESULT CBoots::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Add_EquipList("PlateBoots");
	Add_EquipList("LeatherBoots");
	Add_EquipList("JusinBoots");

	return S_OK;
}

void CBoots::Update(_double deltaTime)
{
}

void CBoots::LateUpdate(_double deltaTime)
{
}

void CBoots::Render()
{
}

CBoots * CBoots::Create()
{
	CBoots*		pInstance = new CBoots();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CBoots");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoots::Free()
{
}