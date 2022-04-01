#include "stdafx.h"
#include "..\Public\Lower.h"

USING(Client)

CLower::CLower()
{
}

CLower::CLower(CGameObject * pObj)
{
}

HRESULT CLower::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Add_EquipList("LeatherLower");
	Add_EquipList("PlateLower");
	Add_EquipList("JusinLower");

	return S_OK;
}

void CLower::Update(_double deltaTime)
{
}

void CLower::LateUpdate(_double deltaTime)
{
}

void CLower::Render()
{
}

CLower * CLower::Create()
{
	CLower*		pInstance = new CLower();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CLower");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLower::Free()
{
}