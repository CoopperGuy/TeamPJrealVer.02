#include "stdafx.h"
#include "..\Public\Pummel_foot.h"
#include "EventCheck.h"
#include "Obb.h"

USING(Client)

CPummel_foot::CPummel_foot()
{
}

HRESULT CPummel_foot::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	m_pThis = pThis;
	m_pTargetTransform = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_vector vTargetPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	_matrix mat = m_pTargetTransform->GetWorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	m_pEffectTrans->SetMatrix(mat);
	m_pEffectTrans->SetUpRotation(m_pEffectTrans->GetState(CTransform::STATE_RIGHT),90.f);
	vTargetPos += m_pTargetTransform->GetState(CTransform::STATE_LOOK) * 0.5f;
	vTargetPos = XMVectorSetY(vTargetPos, 0.5f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);

	return S_OK;
}

void CPummel_foot::Update(_double dDeltaTime)
{
	_vector vTargetPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	vTargetPos += m_pTargetTransform->GetState(CTransform::STATE_LOOK) * 0.5f;
	vTargetPos = XMVectorSetY(vTargetPos, 0.5f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
}

void CPummel_foot::LateUpdate(_double dDeltaTime)
{
	m_DurationDelta += (_float)dDeltaTime;

	if (m_DurationDelta > 0.3f)
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CPummel_foot::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CPummel_foot*		pInstance = new CPummel_foot();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CPummel_foot");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPummel_foot::Free()
{
}
