#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMeteoFire.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectMeteoFire::CEffectMeteoFire()
{
}

CEffectMeteoFire * CEffectMeteoFire::Create(void * pArg)
{
	CEffectMeteoFire*		pInstance = new CEffectMeteoFire();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectMeteoFire");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMeteoFire::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		////flogas
		//CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		//if (pTargetObj == nullptr)
		//	return E_FAIL;

		//CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		//CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));

		//_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger2");

		//m_pTransform->SetMatrix(targetbone *pTargetTransform->GetWorldMatrix());

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
		pos = XMVectorSetZ(pos, XMVectorGetZ(pos) + 0.5f);


		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectMeteoFire::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
	{
		m_bDead = true;
	}

	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));
}


void CEffectMeteoFire::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMeteoFire::Render()
{
}


void CEffectMeteoFire::Free()
{
	__super::Free();
}

