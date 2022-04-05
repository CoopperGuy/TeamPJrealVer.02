#include "stdafx.h"
#include "..\Public\DashAtt.h"
#include "EventCheck.h"
#include "Obb.h"
#include "Dashparticle.h"
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
	mat = m_pTargetTransform->GetWorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]) * m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);
	mat.r[1] = XMVector3Normalize(mat.r[1]) * m_pEffectTrans->GetScale(CTransform::STATE_UP);
	mat.r[2] = XMVector3Normalize(mat.r[2]) * m_pEffectTrans->GetScale(CTransform::STATE_LOOK);
	//vTargetPos += m_pTargetTransform->GetState(CTransform::STATE_LOOK) * 0.05f;
	vTargetPos = XMVectorSetY(vTargetPos, -0.1f);
	mat.r[3] = vTargetPos;
	XMStoreFloat3(&m_vLook, m_pTargetTransform->GetState(CTransform::STATE_LOOK));
	m_pEffectTrans->SetMatrix(mat);

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

	if (m_DurationDelta > 0.2f)
	{
		if (m_bCreate)
		{
			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_Dashparticle", "E_Dashparticle", &m_pEffectTrans->GetWorldMatrix());
			CEngine::GetInstance()->AddScriptObject(CDashparticle::Create((CEmptyEffect*)pGameObject), CEngine::GetInstance()->GetCurSceneNumber());
			m_bCreate = false;
		}
	}
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
