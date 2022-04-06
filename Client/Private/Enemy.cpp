#include "stdafx.h"
#include "..\Public\Enemy.h"
#include "PathFinding.h"
#include "ItemDropEffect.h"

USING(Client)
CEnemy::CEnemy()
{
}
CEnemy::CEnemy(CGameObject * pObj)
	:CCharacter(pObj)
{
}

CEnemy * CEnemy::Create(CGameObject * pObj)
{
	CEnemy*		pInstance = new CEnemy(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CEnemy");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEnemy::Free()
{


}

HRESULT CEnemy::Initialize()
{


	return S_OK;
}

void CEnemy::Update(_double deltaTime)
{
	__super::Update(deltaTime);
}

void CEnemy::LateUpdate(_double deltaTime)
{
	__super::LateUpdate(deltaTime);
}

void CEnemy::Render()
{
}

void CEnemy::SetInitialPosition(_vector position)
{
	PxExtendedVec3 pos = {XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position) };
	m_pController->setPosition(pos);
}




