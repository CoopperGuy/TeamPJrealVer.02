#include "stdafx.h"
#include "..\Public\ImpactShort.h"

USING(Client)

CImpactShort::CImpactShort()
{
}

HRESULT CImpactShort::Initialize(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;

	CTransform* pPlayerTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform"));

	_vector position = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.5f, 1.f), pPlayerTrans->GetWorldMatrix());
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, position);


	return S_OK;
}

void CImpactShort::Update(_double deltaTime)
{
	m_pEffectTrans->SetScale(_float3((_float)m_DurationDelta * 2.f + 2.f, 0.5f, (_float)m_DurationDelta * 2.f + 2.f));
}

void CImpactShort::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
	}
}

CImpactShort * CImpactShort::Create(CEmptyEffect * _pThisEffect, CGameObject * _pPlayerEffect)
{
	CImpactShort*		pInstance = new CImpactShort();

	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayerEffect)))
	{
		MSG_BOX("Failed to Create CImpactShort");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CImpactShort::Free()
{
}

