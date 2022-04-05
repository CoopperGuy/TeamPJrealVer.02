#include "stdafx.h"
#include "..\Public\DashAtt.h"
#include "EventCheck.h"
#include "Obb.h"

USING(Client)

CDashAtt::CDashAtt()
{
}

HRESULT CDashAtt::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	m_pThis = pThis;
	m_pTargetTransform = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_vector vTargetPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	_matrix mat = XMMatrixIdentity();
	m_pTargetTransform->GetWorldMatrix();
	mat.r[0] *= m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);
	mat.r[1] *= m_pEffectTrans->GetScale(CTransform::STATE_UP);
	mat.r[2] *= m_pEffectTrans->GetScale(CTransform::STATE_LOOK);
	vTargetPos += m_pTargetTransform->GetState(CTransform::STATE_LOOK) * 0.5f;
	vTargetPos = XMVectorSetY(vTargetPos, 0.5f);
	XMStoreFloat3(&m_vLook, m_pTargetTransform->GetState(CTransform::STATE_LOOK));
	mat.r[3] = vTargetPos;
	m_pEffectTrans->SetMatrix(mat);
	m_pEffectTrans->SetUpRotation(m_pEffectTrans->GetState(CTransform::STATE_RIGHT),90.f);

	return S_OK;
}

void CDashAtt::Update(_double dDeltaTime)
{
	_vector vTargetPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	vTargetPos += XMLoadFloat3(&m_vLook) * 0.5f;
	vTargetPos = XMVectorSetY(vTargetPos, 0.5f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
}

void CDashAtt::LateUpdate(_double dDeltaTime)
{
	m_DurationDelta += (_float)dDeltaTime;

	if (m_DurationDelta > 0.3f)
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CDashAtt::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CDashAtt*		pInstance = new CDashAtt();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CDashAtt");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDashAtt::Free()
{
}
