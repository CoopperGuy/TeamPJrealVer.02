#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectPhase2Twist.h"
#include "EffectPhase2Inst.h"

USING(Client)

CEffectPahse2Twist::CEffectPahse2Twist()
{
}

CEffectPahse2Twist * CEffectPahse2Twist::Create(void * pArg, CTransform* pTransform, CModel* pModel)
{
	CEffectPahse2Twist*		pInstance = new CEffectPahse2Twist();

	if (FAILED(pInstance->Initialize(pArg, pTransform, pModel)))
	{
		MSG_BOX("Failed to Create CEffectPahse2Twist");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectPahse2Twist::Initialize(void* pArg, CTransform* pTransform, CModel* pModel)
{
	if (pArg != nullptr) 
	{
		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
								
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		m_pTargetTransform = pTransform;
		m_pTargetModel = pModel;
		
		_matrix pTransform = m_pTargetModel->Get_BoneWithoutOffset("Bip01") * m_pTargetTransform->GetWorldMatrix();
		_float4 vPosition;
		memcpy(&vPosition, &pTransform.r[3], sizeof(_float4));		

		m_pTransform->SetState(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));
	}

	return S_OK;
}

void CEffectPahse2Twist::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
		
	m_fInstanceSpawnTimeAcc += (_float)deltaTime;
	
	_matrix pTransform = m_pTargetModel->Get_BoneWithoutOffset("Bip01") * m_pTargetTransform->GetWorldMatrix();
	_float4 vPosition;
	memcpy(&vPosition, &pTransform.r[3], sizeof(_float4));

	m_pTransform->SetState(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));

	/*if (m_fInstanceSpawnDelay <= m_fInstanceSpawnTimeAcc)
	{
		CEngine::GetInstance()->AddScriptObject(CEffectPahse2Inst::Create(m_pTransform), CEngine::GetInstance()->GetCurSceneNumber());
		m_fInstanceSpawnTimeAcc = 0.f;
	}*/
}

void CEffectPahse2Twist::LateUpdate(_double deltaTime)
{	
	
}

void CEffectPahse2Twist::Render()
{
}

void CEffectPahse2Twist::End_Effect()
{
	this->SetDead();
	m_pGameObject->SetDead();
}


void CEffectPahse2Twist::Free()
{
	__super::Free();
}
