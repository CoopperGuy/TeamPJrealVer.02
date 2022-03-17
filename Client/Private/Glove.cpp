#include "stdafx.h"
#include "..\Public\Glove.h"

USING(Client)

CGlove::CGlove()
{
}

CGlove::CGlove(CGameObject * pObj)
{
}

HRESULT CGlove::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Add_EquipList("Gauntlet");
	Add_EquipList("LeatherGlove");

	return S_OK;
}

void CGlove::Update(_double deltaTime)
{
}

void CGlove::LateUpdate(_double deltaTime)
{
}

void CGlove::Render()
{
}

CGlove * CGlove::Create()
{
	CGlove*		pInstance = new CGlove();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CGlove");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGlove::Free()
{
}