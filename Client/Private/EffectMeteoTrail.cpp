#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMeteoTrail.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectMeteoTrail::CEffectMeteoTrail()
{
}

CEffectMeteoTrail * CEffectMeteoTrail::Create(void * pArg)
{
	CEffectMeteoTrail*		pInstance = new CEffectMeteoTrail();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectMeteoTrail");
		SafeRelease(pInstance);
		return nullptr;
	}


	return pInstance;
}


HRESULT CEffectMeteoTrail::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		//flogas
		CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));

		//_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger2");
		//targetbone = Remove_ScaleRotation(targetbone *pTargetTransform->GetWorldMatrix());
		//m_pTransform->SetMatrix(targetbone);

		_vector pos = pTargetTransform->GetState(CTransform::STATE_POSITION);
		pos = XMVectorSetZ(pos, XMVectorGetZ(pos) - 0.3f);


		m_pTransform->SetState(CTransform::STATE_POSITION, pos);


	}
	return S_OK;
}

void CEffectMeteoTrail::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	m_dFadeOutDuration += deltaTime;
	
	if (m_dFadeOutDuration >= static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutDuration())
	{
		m_bDead = true;
	}


}


void CEffectMeteoTrail::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMeteoTrail::Render()
{
}


void CEffectMeteoTrail::Free()
{
	__super::Free();
}

_fmatrix CEffectMeteoTrail::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}