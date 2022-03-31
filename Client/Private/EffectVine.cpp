#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectVine.h"
#include "EmptyEffect.h"
#include "Player.h"

USING(Client)

CEffectVine::CEffectVine()
{
}

CEffectVine * CEffectVine::Create(void * pArg)
{
	CEffectVine*		pInstance = new CEffectVine();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectVine");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectVine::Initialize(void* pArg)
{
	if (pArg != nullptr) {


		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;


		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));


		CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
		CTransform* m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));

		m_pTransform->SetState(CTransform::STATE_POSITION, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
	}
	return S_OK;
}

void CEffectVine::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	DeadDt += deltaTime;
}

void CEffectVine::LateUpdate(_double deltaTime)
{


	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration() <= DeadDt)
	{
		//CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	//	dynamic_cast<CPlayer*>(pTargetObj)->SetSpeed(1.5f);

		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectVine::Render()
{
}

void CEffectVine::Free()
{
	__super::Free();
}
