#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectEAFire.h"
#include "EmptyEffect.h"
#include "EffectFireBall.h"

USING(Client)

CEffectEAFire::CEffectEAFire()
{
}

CEffectEAFire * CEffectEAFire::Create(void * pArg)
{
	CEffectEAFire*		pInstance = new CEffectEAFire();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectEAFire");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectEAFire::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	}
	return S_OK;
}

void CEffectEAFire::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dMakeFB + -deltaTime;
	_vector pPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	if (m_dMakeFB >= 0.8)
	{
		auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireBall", "E_EAFireBall");
		CEngine::GetInstance()->AddScriptObject(CEffectFireBall::Create(EffectFireBall, pPos), CEngine::GetInstance()->GetCurSceneNumber());
		m_dMakeFB = 0;
	}
}


void CEffectEAFire::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectEAFire::Render()
{
}

void CEffectEAFire::Free()
{
	__super::Free();
}
