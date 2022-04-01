#include "stdafx.h"
#include "..\Public\Armor.h"

USING(Client)

CArmor::CArmor()
{
}

CArmor::CArmor(CGameObject * pObj)
{
}

HRESULT CArmor::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Add_EquipList("PlateArmor");
	Add_EquipList("LeatherArmor");
	Add_EquipList("JusinArmor");

	return S_OK;
}

void CArmor::Update(_double deltaTime)
{
}

void CArmor::LateUpdate(_double deltaTime)
{
}

void CArmor::Render()
{
}

CArmor * CArmor::Create()
{
	CArmor*		pInstance = new CArmor();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CArmor");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CArmor::Free()
{
}