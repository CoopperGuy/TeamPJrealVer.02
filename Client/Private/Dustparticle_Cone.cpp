#include "stdafx.h"
#include "..\Public\Dustparticle_Cone.h"
#include "EventCheck.h"
#include "Obb.h"

USING(Client)

CDustparticle_Cone::CDustparticle_Cone()
{
}

HRESULT CDustparticle_Cone::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	m_pThis = pThis;
	//CTransform* pTargetTransform = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	//CTransform* pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	//CModel* pModel = static_cast<CModel*>(pTarget->GetComponent("Com_Model"));
	m_pParticle = static_cast<CVIBuffer_RectInstance*>(m_pThis->GetComponent("Com_RectInstance"));
	//_matrix mat = XMMatrixIdentity();
	//mat.r[3] = (pModel->Get_BoneMatrix("Bip01-L-Foot") * pTargetTransform->GetWorldMatrix()).r[3];
	//_vector vTargetPos = mat.r[3];
	//pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);

	return S_OK;
}

void CDustparticle_Cone::Update(_double dDeltaTime)
{
	m_DurationDelta += (_float)dDeltaTime;
}

void CDustparticle_Cone::LateUpdate(_double dDeltaTime)
{

	if (m_DurationDelta >= m_pParticle->Get_LifeTime())
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CDustparticle_Cone::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CDustparticle_Cone*		pInstance = new CDustparticle_Cone();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CDustparticle_Cone");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDustparticle_Cone::Free()
{
}
