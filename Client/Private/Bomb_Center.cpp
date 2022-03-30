#include "stdafx.h"
#include "..\Public\Bomb_Center.h"
#include "Element_Bomb.h"

USING(Client)

CBomb_Center::CBomb_Center()
{
}

HRESULT CBomb_Center::Initialize(CEmptyEffect* pThis, CTransform* pTarget, CElement_Bomb& pBomb)
{
	
	m_pThis = pThis;
	m_pBomb = &pBomb;
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	list<CGameObject*>Child = m_pThis->GetChildren();
	for (auto pChildTransform : Child)
	{
		CTransform* pTrans = static_cast<CTransform*>(pChildTransform->GetComponent("Com_Transform"));
		if (pTrans)
			m_Child.emplace_back(pTrans);
	}
	_float fscale = m_Child.front()->GetScale(CTransform::STATE_RIGHT);
	m_vScale = { fscale, fscale, fscale };
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, pTarget->GetState(CTransform::STATE_POSITION));
	return S_OK;
}

void CBomb_Center::Update(_double dDeltaTime)
{
	m_fScale -= (_float)dDeltaTime*0.5f;
	if (m_fScale < 0.5f)
		m_fScale = 1.f;
		m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, m_fScale));

	for (auto pChildTransform : m_Child)
	{
		if (pChildTransform)
			pChildTransform->SetScale(_float3(m_vScale.x * m_fScale, m_vScale.y * m_fScale, m_vScale.z * m_fScale));
	}
}

void CBomb_Center::LateUpdate(_double deltaTime)
{
	if (m_pBomb->Get_Release())
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CBomb_Center::Create(CEmptyEffect* pThis, CTransform* pTarget, CElement_Bomb& pBomb)
{
	CBomb_Center*		pInstance = new CBomb_Center();

	if (FAILED(pInstance->Initialize(pThis, pTarget, pBomb)))
	{
		MSG_BOX("Failed to Create CBomb_Center");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBomb_Center::Free()
{
}
