#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectEAFire.h"
#include "EmptyEffect.h"
#include "EffectFireBall.h"

USING(Client)

CEffectEAFire::CEffectEAFire()
{
}

CEffectEAFire * CEffectEAFire::Create(void * pArg,_vector pos)
{
	CEffectEAFire*		pInstance = new CEffectEAFire();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectEAFire");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectEAFire::Initialize(void* pArg,_vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	}
	return S_OK;
}

void CEffectEAFire::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dMakeFB += deltaTime;
	vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
	if (m_dMakeFB >= 0.15)
	{
		auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Fire", "E_EAFire");
		CEngine::GetInstance()->AddScriptObject(CEffectFireBall::Create(EffectFireBall, vPosition), CEngine::GetInstance()->GetCurSceneNumber());
		m_dMakeFB = 0;
	}

}


void CEffectEAFire::LateUpdate(_double deltaTime)
{
	if (0.1f >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
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

void CEffectEAFire::Set_Pos(_vector pos)
{
	_vector postemp = {};
	memcpy(&postemp, &pos, sizeof(XMVECTOR));

	m_pTransform->SetState(CTransform::STATE_POSITION, postemp);
}
