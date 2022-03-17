#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectFireTwist.h"
#include "EmptyEffect.h"

USING(Client)

CEffectFireTwist::CEffectFireTwist()
{
}

CEffectFireTwist * CEffectFireTwist::Create(void * pArg)
{
	CEffectFireTwist*		pInstance = new CEffectFireTwist();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectFireTwist");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectFireTwist::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));


	pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;


		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));

		_matrix posmatrix = {};
		_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger3");
		m_pTransform->SetMatrix(targetbone * pTargetTransform->GetWorldMatrix()) ;
	}
	return S_OK;
}

void CEffectFireTwist::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;


	if (pTargetObj) {
		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));
		_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger3");
		m_pTransform->SetMatrix(targetbone * pTargetTransform->GetWorldMatrix());
	}
}

void CEffectFireTwist::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectFireTwist::Render()
{
}

void CEffectFireTwist::SetGameObject(CGameObject* obj)
{
	m_pGameObject = obj;
}
void CEffectFireTwist::SetTransform(CTransform * trans)
{
	m_pTransform = trans;
}

void CEffectFireTwist::Free()
{
	__super::Free();
}
