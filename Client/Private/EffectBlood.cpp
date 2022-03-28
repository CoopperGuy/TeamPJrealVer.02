#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectBlood.h"

USING(Client)

CEffectBlood::CEffectBlood()
{
}

CEffectBlood * CEffectBlood::Create(void * pArg)
{
	CEffectBlood*		pInstance = new CEffectBlood();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectBlood");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectBlood::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		m_pComponent = m_pGameObject->GetComponent("Com_RectInstance");


		RandomNum = rand() % 3;
	}
	return S_OK;
}

void CEffectBlood::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;

}

void CEffectBlood::LateUpdate(_double deltaTime)
{
	//if (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration()<= m_dDeadTime)
	if (static_cast<CVIBuffer_RectInstance*>(m_pComponent)->Get_LifeTime() <= m_dDeadTime)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectBlood::Render()
{
}


void CEffectBlood::Free()
{
	__super::Free();
}
