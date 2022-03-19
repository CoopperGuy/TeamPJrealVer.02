#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMeteoFireBall.h"
#include "EmptyEffect.h"
#include "EffectMeteoExpolRing.h"
#include "EffectMeteoFire.h"
USING(Client)

CEffectMeteoFireBall::CEffectMeteoFireBall()
{
}

CEffectMeteoFireBall * CEffectMeteoFireBall::Create(void * pArg, _vector* pos)
{
	CEffectMeteoFireBall*		pInstance = new CEffectMeteoFireBall();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMeteoFireBall");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMeteoFireBall::Initialize(void* pArg, _vector* pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		m_pTransform->SetState(CTransform::STATE_POSITION, *pos);

		startposy = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));
	}
	return S_OK;
}

void CEffectMeteoFireBall::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	posy = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));
	pos = m_pTransform->GetState(CTransform::STATE_POSITION);
	deaddt += deltaTime;
	

	if (deaddt >=0.5f)
	{
		deaddt = 0;
		m_bDead = true;
	}
}


void CEffectMeteoFireBall::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		if (posy <= 0.2) {

			auto EffectTrail = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoEnd", "E_MeteoEnd");
			CEngine::GetInstance()->AddScriptObject(CEffectMeteoExpolRing::Create(EffectTrail, pos), CEngine::GetInstance()->GetCurSceneNumber());
		}
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMeteoFireBall::Render()
{
}


void CEffectMeteoFireBall::Free()
{
	__super::Free();
}

void CEffectMeteoFireBall::SetPos(_matrix _pmatrix)
{
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetMatrix(_pmatrix);
	}
}

void CEffectMeteoFireBall::Set_Pos(_vector pPos)
{
}
