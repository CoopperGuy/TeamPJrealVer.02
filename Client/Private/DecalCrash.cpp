#include "stdafx.h"
#include "..\Public\DecalCrash.h"

USING(Client)

CDecalCrash::CDecalCrash()
{
}

HRESULT CDecalCrash::Initialize(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;

	CTransform* pPlayerTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform"));

	_vector position = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.5f, 1.f), pPlayerTrans->GetWorldMatrix());
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, position);

	return S_OK;
}

void CDecalCrash::Update(_double deltaTime)
{
}

void CDecalCrash::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
	}
}

CDecalCrash * CDecalCrash::Create(CEmptyEffect * _pThisEffect, CGameObject * _pPlayerEffect)
{
	CDecalCrash*		pInstance = new CDecalCrash();

	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayerEffect)))
	{
		MSG_BOX("Failed to Create CDecalCrash");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDecalCrash::Free()
{
}
