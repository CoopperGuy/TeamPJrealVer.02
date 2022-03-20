#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectBlackhole.h"
#include "EmptyEffect.h"

USING(Client)

CEffectBlackhole::CEffectBlackhole()
{
}

CEffectBlackhole * CEffectBlackhole::Create(void * pArg)
{
	CEffectBlackhole*		pInstance = new CEffectBlackhole();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectBlackhole");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectBlackhole::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));


		//flogas
		pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));

		_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger2");
		//memccpy(&m_pTransform->GetWorldMatrix(), &targetbone, 0, sizeof(_matrix));
		m_pTransform->SetMatrix(targetbone *pTargetTransform->GetWorldMatrix());
	}
	return S_OK;
}

void CEffectBlackhole::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadDt += deltaTime;
	if (m_dDeadDt >=3)
		m_bDead = true;


	CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
	CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));
	_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger2");
	m_pTransform->SetMatrix(targetbone *pTargetTransform->GetWorldMatrix());
}

void CEffectBlackhole::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectBlackhole::Render()
{
}

void CEffectBlackhole::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(1.5f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(1.5f);
}

void CEffectBlackhole::Free()
{
	__super::Free();
}
