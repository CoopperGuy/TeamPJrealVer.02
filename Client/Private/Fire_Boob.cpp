#include "stdafx.h"
#include "..\Public\Fire_Boob.h"

USING(Client)

CFire_Boob::CFire_Boob()
{
}

HRESULT CFire_Boob::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	
	m_pThis = pThis;
	m_pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_vector vTargetPos = m_pTargetTrans->GetState(CTransform::STATE_POSITION);
	vTargetPos += m_pTargetTrans->GetState(CTransform::STATE_LOOK) * 0.5f;
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	return S_OK;
}

void CFire_Boob::Update(_double dDeltaTime)
{

}

void CFire_Boob::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta >  m_pThis->GetFadeOutDuration()) 
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CFire_Boob::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CFire_Boob*		pInstance = new CFire_Boob();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CFire_Boob");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFire_Boob::Free()
{
}
