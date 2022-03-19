#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMagic.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
USING(Client)

CEffectMagic::CEffectMagic()
{
}

CEffectMagic * CEffectMagic::Create(void * pArg, _vector pos)
{
	CEffectMagic*		pInstance = new CEffectMagic();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMagic");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMagic::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		_vector mypos = m_pTransform->GetState(CTransform::STATE_POSITION);

		int r = rand() % 2;
		r += 1;
		int pRandom;
		if (r == 1) {
			pRandom = rand() % 10;
			mypos = XMVectorSet(XMVectorGetX(pos) - pRandom, 0.1f, XMVectorGetZ(pos) - pRandom, 1.f);
		}
		else {
			pRandom = (rand() % 10)* -1;
			mypos = XMVectorSet(XMVectorGetX(pos) + pRandom, 0.1f, XMVectorGetZ(pos) + pRandom, 1.f);

		}



		m_pTransform->SetState(CTransform::STATE_POSITION, mypos);

	}
	return S_OK;
}

void CEffectMagic::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	
	m_dDeadTime += deltaTime;

	_vector mypos = m_pTransform->GetState(CTransform::STATE_POSITION);

	if (m_dDeadTime >=static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
	/*	auto EffectTrail = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoDropAf", "E_MeteoDropAfter");
		CEngine::GetInstance()->AddScriptObject(CEffectMagicAf::Create(EffectTrail, mypos), CEngine::GetInstance()->GetCurSceneNumber());*/
		m_bDead = true;
	}
}


void CEffectMagic::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMagic::Render()
{
}


void CEffectMagic::Free()
{
	__super::Free();
}
