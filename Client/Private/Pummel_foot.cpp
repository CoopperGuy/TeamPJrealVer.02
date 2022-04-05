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
	CModel* pModel = static_cast<CModel*>(pTarget->GetComponent("Com_Model"));
	_matrix mat = XMMatrixIdentity();
	mat.r[3] = (pModel->Get_BoneMatrix("Bip01-L-Foot") * m_pTargetTransform->GetWorldMatrix()).r[3];
	mat.r[0] = mat.r[0] * m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);
	mat.r[1] = mat.r[1] * m_pEffectTrans->GetScale(CTransform::STATE_UP);
	mat.r[2] = mat.r[2] * m_pEffectTrans->GetScale(CTransform::STATE_LOOK);

	m_pEffectTrans->SetMatrix(mat);
	m_pEffectTrans->SetLook(-m_pTargetTransform->GetState(CTransform::STATE_LOOK));
	_vector vTargetPos = m_pEffectTrans->GetState(CTransform::STATE_POSITION);
	//vTargetPos = XMVectorSetY(vTargetPos, 0.1f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);

	return S_OK;
}

void CPummel_foot::Update(_double dDeltaTime)
{
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
