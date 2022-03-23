#include "stdafx.h"
#include "..\Public\SlashWave.h"
#include "Obb.h"
USING(Client)

CSlashWave::CSlashWave()
{
}

HRESULT CSlashWave::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	
	m_pThis = pThis;

	m_pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	_vector vLook = m_pTargetTrans->GetState(CTransform::STATE_LOOK);
	m_pEffectTrans->SetUpRotation(XMVectorSet(0.f,0.f,1.f,0.f), 60.f);
	m_pEffectTrans->SetLookUpVector(vLook);
	_vector vTargetPos = m_pTargetTrans->GetState(CTransform::STATE_POSITION);
	const _vector	vOffset = XMVectorSet(-0.3f, -0.1f, 0.f, 0.f);
	m_pChildren = static_cast<CTransform*>(m_pThis->GetChildren().front()->GetComponent("Com_LocalTransform"));
//	vTargetPos = XMVectorSetY(vTargetPos, -0.1f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	_vector vPos = m_pEffectTrans->GetState(CTransform::STATE_POSITION);
	//m_pEffectTrans->GoRight(0.05);
	vPos += XMVector3Normalize(m_pTargetTrans->GetState(CTransform::STATE_RIGHT))  * -0.05f;


	CTransform* pChildTrans = static_cast<CTransform*>(m_pThis->GetChildren().front()->GetComponent("Com_Transform"));
	m_vChildScale = { pChildTrans->GetScale(CTransform::STATE_RIGHT),pChildTrans->GetScale(CTransform::STATE_UP), pChildTrans->GetScale(CTransform::STATE_LOOK) };

	_vector pos = vTargetPos;
	pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.2f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, pos);

	CStat* stat = static_cast<CStat*>(pTarget->GetComponent("Com_Stat"));

	m_pOBB = CObb::Create(pos, XMLoadFloat3(&m_vChildScale), stat->GetStatInfo().atk, ID::MONSTER_EFFECT, 100.f, nullptr);

	return S_OK;
}

void CSlashWave::Update(_double dDeltaTime)
{
	if (m_DurationDelta > 0.2)
	{
		m_pChildren->GetScale(CTransform::STATE_RIGHT);
		m_pEffectTrans->GoStraight(dDeltaTime * 1.0);
		m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, m_fScale));

		m_pChildren->SetScale(_float3(m_vChildScale.x *m_fScale, m_vChildScale.y *m_fScale, m_vChildScale.z *m_fScale));
		

		_vector pos = m_pEffectTrans->GetState(CTransform::STATE_POSITION);
		_float3 effectPos;
		XMStoreFloat3(&effectPos, pos);

		if(m_fScale < 1.5f)
			m_fScale += (_float)dDeltaTime;

		
		m_pOBB->SetPosision(effectPos);
	}
}

void CSlashWave::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		m_pOBB->SetupDead();
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CSlashWave::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CSlashWave*		pInstance = new CSlashWave();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CSlashWave");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSlashWave::Free()
{
}
