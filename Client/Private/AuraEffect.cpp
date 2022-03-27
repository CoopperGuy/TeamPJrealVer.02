#include "stdafx.h"
#include "..\Public\AuraEffect.h"

USING(Client)

CAuraEffect::CAuraEffect()
{
}

HRESULT CAuraEffect::Initialize(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;

	CTransform* pPlayerTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform"));

	_vector position = XMVector3TransformCoord(XMVectorSet(0.f, 0.5f, 0.f, 1.f), pPlayerTrans->GetWorldMatrix());
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, position);

	return S_OK;
}

void CAuraEffect::Update(_double deltaTime)
{
}

void CAuraEffect::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
	}
}

CAuraEffect * CAuraEffect::Create(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	CAuraEffect*		pInstance = new CAuraEffect();

	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayer)))
	{
		MSG_BOX("Failed to Create CAuraEffect");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAuraEffect::Free()
{
}

