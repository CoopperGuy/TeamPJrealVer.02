#include "stdafx.h"
#include "..\Public\FireExplosion.h"

USING(Client)

CFireExplosion::CFireExplosion()
{
}

HRESULT CFireExplosion::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	
	m_pThis = pThis;

	m_pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	_vector vTargetPos = m_pTargetTrans->GetState(CTransform::STATE_POSITION);
	m_pChildren = static_cast<CTransform*>(m_pThis->GetChildren().front()->GetComponent("Com_Transform"));
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	_vector vPos = m_pEffectTrans->GetState(CTransform::STATE_POSITION);
	return S_OK;
}

void CFireExplosion::Update(_double dDeltaTime)
{
	if (m_DurationDelta > 0.2)
	{
		m_pEffectTrans->SetScale(_float3(m_fScale - 0.5f , m_fScale, 1.f));
		m_pChildren->SetScale(_float3(m_fScale, m_fScale, 1.f));
		m_fScale += dDeltaTime;
	}
}

void CFireExplosion::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CFireExplosion::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CFireExplosion*		pInstance = new CFireExplosion();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CFireExplosion");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFireExplosion::Free()
{
}
