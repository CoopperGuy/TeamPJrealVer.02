#include "stdafx.h"
#include "..\Public\FootHammer_Bomb.h"
#include "EventCheck.h"

USING(Client)

CFootHammer_Bomb::CFootHammer_Bomb()
{
}

HRESULT CFootHammer_Bomb::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	m_pThis = pThis;
	CTransform* pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_float3 vTargetPos;
	XMStoreFloat3(&vTargetPos, pTargetTrans->GetState(CTransform::STATE_POSITION));
	m_fScale = m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);
	vTargetPos.y = 1.f;
	
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&vTargetPos));

	return S_OK;
}

void CFootHammer_Bomb::Update(_double dDeltaTime)
{
	m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, 1.f));
	m_fScale += (_float)dDeltaTime * 0.5f;
}

void CFootHammer_Bomb::LateUpdate(_double dDeltaTime)
{
	if(m_DurationDelta <= 0)
		CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 4, 0.02f);
	m_DurationDelta += (_float)dDeltaTime;

	if (m_pThis->GetSpriteEnd())
	{

		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CFootHammer_Bomb::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CFootHammer_Bomb*		pInstance = new CFootHammer_Bomb();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CFootHammer_Bomb");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFootHammer_Bomb::Free()
{
}
