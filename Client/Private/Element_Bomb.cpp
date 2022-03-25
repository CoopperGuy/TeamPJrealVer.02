#include "stdafx.h"
#include "..\Public\Element_Bomb.h"

USING(Client)

CElement_Bomb::CElement_Bomb()
{
}

HRESULT CElement_Bomb::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	
	m_pThis = pThis;
	m_pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_vector vTargetPos = m_pTargetTrans->GetState(CTransform::STATE_POSITION);
	//vTargetPos += m_pTargetTrans->GetState(CTransform::STATE_LOOK) * 0.5f;
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	return S_OK;
}

void CElement_Bomb::Update(_double dDeltaTime)
{

}

void CElement_Bomb::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta >  m_pThis->GetFadeOutDuration()) 
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CElement_Bomb::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CElement_Bomb*		pInstance = new CElement_Bomb();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CElement_Bomb");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CElement_Bomb::Free()
{
}
