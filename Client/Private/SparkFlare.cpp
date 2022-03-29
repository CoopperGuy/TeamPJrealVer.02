#include "stdafx.h"
#include "..\Public\SparkFlare.h"

USING(Client)

CSparkFlare::CSparkFlare()
{
}

HRESULT CSparkFlare::Initialize(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;

	m_pPlayerTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform"));

	_vector position = m_pPlayerTrans->GetState(CTransform::STATE_POSITION);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, position);


	return S_OK;
}

void CSparkFlare::Update(_double deltaTime)
{
}

void CSparkFlare::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
	}
}

CSparkFlare * CSparkFlare::Create(CEmptyEffect * _pThisEffect, CGameObject * _pPlayerEffect)
{
	CSparkFlare*		pInstance = new CSparkFlare();

	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayerEffect)))
	{
		MSG_BOX("Failed to Create CSparkFlare");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSparkFlare::Free()
{
}


