#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\ItemDropEffect.h"
#include "EmptyEffect.h"

USING(Client)

CItemDropEffect::CItemDropEffect()
{
}

CItemDropEffect * CItemDropEffect::Create(void * pArg, _vector pos)
{
	CItemDropEffect*		pInstance = new CItemDropEffect();

	if (FAILED(pInstance->Initialize(pArg,pos)))
	{
		MSG_BOX("Failed to Create CItemDropEffect");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CItemDropEffect::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));


	//	pos = XMVectorSetY(pos, 0.f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	}
	return S_OK;
}

void CItemDropEffect::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

}

void CItemDropEffect::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CItemDropEffect::Render()
{
}


void CItemDropEffect::Free()
{
	__super::Free();
}
