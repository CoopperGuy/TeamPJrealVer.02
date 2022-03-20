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
		//pos = XMVectorSetZ(pos, XMVectorGetZ(pos) - 0.3f);


		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
		CTransform* pPlayerTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));

		m_pTransform->SetLook(pPlayerTransform->GetState(CTransform::STATE_POSITION));

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

void CEffectMeteoTrail::LookAt(_fvector vTargetPos)
{
	_vector		vDirection = vTargetPos - m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y축 // 외적으로 방향백터를 구하기위해서 그리고 좌우로만 바뀌지 y축은 안바뀌니까
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//위에서 외적한 right는 스케일이 깨져있어서 원래 사용하던 right를 대입해주자 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// 위에서 바꿔준 항등행렬과 y축을 외적하기 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);
}