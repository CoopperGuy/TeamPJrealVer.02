#include "stdafx.h"
#include "..\Public\ChargeAxe.h"

USING(Client)

CChargeAxe::CChargeAxe()
{
}

HRESULT CChargeAxe::Initialize(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;

	m_pAxeTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform")); 
	
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

void CChargeAxe::Update(_double deltaTime)
{
	if (m_pEffectTrans && m_pAxeTrans)
	{
		m_fScale -= (_float)deltaTime * 0.6f;
		const _vector	correctPosition = XMVectorSet(0.f, 0.15f, 0.f, 0.f);
		_vector axePos = XMVector3TransformCoord(correctPosition, m_pAxeTrans->GetWorldMatrix());
		m_pEffectTrans->SetState(CTransform::STATE_POSITION, axePos);
		m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, m_fScale));
	}

}

void CChargeAxe::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
	}
}

CChargeAxe * CChargeAxe::Create(CEmptyEffect * _pThisEffect, CGameObject * _pPlayerEffect)
{
	CChargeAxe*		pInstance = new CChargeAxe();

	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayerEffect)))
	{
		MSG_BOX("Failed to Create CChargeAxe");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CChargeAxe::Free()
{
}
