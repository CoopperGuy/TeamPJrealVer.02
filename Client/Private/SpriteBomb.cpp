#include "stdafx.h"
#include "..\Public\SpriteBomb.h"
#include "EventCheck.h"
#include "Obb.h"

USING(Client)

CSpriteBomb::CSpriteBomb()
{
}

HRESULT CSpriteBomb::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	m_pThis = pThis;
	CTransform* pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_float3 vTargetPos;
	XMStoreFloat3(&vTargetPos, pTargetTrans->GetState(CTransform::STATE_POSITION));
	m_vScale = { m_pEffectTrans->GetScale(CTransform::STATE_RIGHT), m_pEffectTrans->GetScale(CTransform::STATE_UP),1.f };
	vTargetPos.y = 0.5f;
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&vTargetPos));
	
	CGameObject* pFlogas = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
	CStat* stat = static_cast<CStat*>(pFlogas->GetComponent("Com_Stat"));
	_float3 vPos = {};
	XMStoreFloat3(&vPos, m_pEffectTrans->GetState(CTransform::STATE_POSITION));

	m_pOBB = CObb::Create(vTargetPos, m_vScale, stat->GetStatInfo().atk, ID::MONSTER_EFFECT, 100.f, nullptr);
	m_pOBB->SetIsDownAttack(true);
	return S_OK;
}

void CSpriteBomb::Update(_double dDeltaTime)
{
	m_pEffectTrans->SetScale(_float3(m_vScale.x + m_fScale, m_vScale.y + m_fScale, 1.f));
	m_fScale += (_float)dDeltaTime * 0.5f;
	m_pOBB->SetIsDownAttack(true);
	_float3 vPos = {};
	XMStoreFloat3(&vPos, m_pEffectTrans->GetState(CTransform::STATE_POSITION));
	vPos.y = 0.f;
	m_pOBB->SetPosision(vPos);
	m_pOBB->SetSize(_float3((m_vScale.x + m_fScale)* 0.5f, (m_vScale.y + m_fScale)* 0.5f, 1.f));
}

void CSpriteBomb::LateUpdate(_double dDeltaTime)
{
	if(m_DurationDelta <= 0)
		CEventCheck::GetInstance()->ShakeUpDown(8, 0.05f);
	m_DurationDelta += (_float)dDeltaTime;

	if (m_pThis->GetSpriteEnd())
	{
		m_pOBB->SetupDead();
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CSpriteBomb::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CSpriteBomb*		pInstance = new CSpriteBomb();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
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
