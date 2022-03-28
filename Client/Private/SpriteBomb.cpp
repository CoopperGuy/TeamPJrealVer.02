#include "stdafx.h"
#include "..\Public\SpriteBomb.h"

USING(Client)

CSpriteBomb::CSpriteBomb()
{
}

HRESULT CSpriteBomb::Initialize(CEmptyEffect* pThis, CGameObject* pTarget, _fvector sortvalue)
{
	m_pThis = pThis;
	CTransform* pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_float3 vTargetPos;
	XMStoreFloat3(&vTargetPos, pTargetTrans->GetState(CTransform::STATE_POSITION));
	m_fScale = m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);
	vTargetPos.y = 0.5f;
	_float X = XMVectorGetX(sortvalue);
	_float Z = XMVectorGetZ(sortvalue);
	vTargetPos.x += X;
	vTargetPos.z += Z;
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&vTargetPos));

	return S_OK;
}

void CSpriteBomb::Update(_double dDeltaTime)
{
	m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, 1.f));
	m_fScale += (_float)dDeltaTime * 0.5f;
}

void CSpriteBomb::LateUpdate(_double dDeltaTime)
{
	m_DurationDelta += (_float)dDeltaTime;
	if (m_DurationDelta > m_pThis->GetFadeOutDuration()) 
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CSpriteBomb::Create(CEmptyEffect* pThis, CGameObject* pTarget, _fvector sortvalue)
{
	CSpriteBomb*		pInstance = new CSpriteBomb();

	if (FAILED(pInstance->Initialize(pThis, pTarget, sortvalue)))
	{
		MSG_BOX("Failed to Create CSpriteBomb");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSpriteBomb::Free()
{
}
