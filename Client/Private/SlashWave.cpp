#include "stdafx.h"
#include "..\Public\SlashWave.h"

USING(Client)

CSlashWave::CSlashWave()
{
}

HRESULT CSlashWave::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	
	m_pThis = pThis;

	m_pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	_vector vLook = m_pTargetTrans->GetState(CTransform::STATE_LOOK);
	m_pEffectTrans->SetLook(vLook);
	_vector vTargetPos = m_pTargetTrans->GetState(CTransform::STATE_POSITION);
	const _vector	vOffset = XMVectorSet(0.5f, 0.f, 0.f, 0.f);
	m_pChildren = static_cast<CTransform*>(m_pThis->GetChildren().front()->GetComponent("Com_Transform"));
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos + vOffset);
	m_pEffectTrans->SetUpRotation(m_pEffectTrans->GetState(CTransform::STATE_RIGHT), 10.f);
	return S_OK;
}

void CSlashWave::Update(_double dDeltaTime)
{
	if (m_DurationDelta > 0.2)
	{
		m_pEffectTrans->GoStraight(dDeltaTime * 1.0);
		m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, m_fScale));
		m_pChildren->SetScale(_float3(m_fScale, m_fScale, m_fScale));
		m_fScale += dDeltaTime;
	}
}

void CSlashWave::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CSlashWave::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CSlashWave*		pInstance = new CSlashWave();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CSlashWave");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSlashWave::Free()
{
}
