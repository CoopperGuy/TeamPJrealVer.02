#include "stdafx.h"
#include "..\Public\ShoulerAtk.h"

USING(Client)

CShoulerAtk::CShoulerAtk()
{
}

HRESULT CShoulerAtk::Initialize(CEmptyEffect* _pThisEffect, CGameObject* _pPlayer)
{
	
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;

	m_pPlayerTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform"));

	return S_OK;
}

void CShoulerAtk::Update(_double deltaTime)
{
	//const _matrix pivot = XMMatrixRotationX(90.f);
	//_matrix effct = XMMatrixMultiply(pivot , m_pPlayerTrans->GetWorldMatrix());
	//m_pEffectTrans->SetMatrix(effct);

	_vector playerLook = m_pPlayerTrans->GetState(CTransform::STATE_LOOK);
	m_pEffectTrans->SetLookUpVector(playerLook);

	_vector playerPos = m_pPlayerTrans->GetState(CTransform::STATE_POSITION);
	const _vector	correctPosition = XMVectorSet(0.f, 0.15f, 0.f, 0.f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, playerPos + correctPosition);
}

void CShoulerAtk::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
	}
}



CBasicEffect * CShoulerAtk::Create(CEmptyEffect* _pThisEffect, CGameObject* _pPlayerEffect)
{
	CShoulerAtk*		pInstance = new CShoulerAtk();

	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayerEffect)))
	{
		MSG_BOX("Failed to Create CShoulerAtk");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CShoulerAtk::Free()
{
}
