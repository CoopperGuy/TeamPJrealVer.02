#include "stdafx.h"
#include "..\Public\Fire_explosion.h"
#include "Fire_Boob.h"
USING(Client)

CFire_explosion::CFire_explosion()
{
}

HRESULT CFire_explosion::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	
	m_pThis = pThis;
	m_pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	m_pChildren = static_cast<CTransform*>(m_pThis->GetChildren().front()->GetComponent("Com_Transform"));
	
	_vector vTargetPos = m_pTargetTrans->GetState(CTransform::STATE_POSITION);
	vTargetPos = XMVectorSetY(vTargetPos, 0.5f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	//_vector vPos = m_pEffectTrans->GetState(CTransform::STATE_POSITION);
	//vPos += XMVector3Normalize(m_pTargetTrans->GetState(CTransform::STATE_LOOK))  * 0.05f;
	//m_pEffectTrans->SetState(CTransform::STATE_POSITION, vPos);
	CEmptyEffect* pBoob = static_cast<CEmptyEffect*>(CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireBoob", "FireBoob"));
	CEngine::GetInstance()->AddScriptObject(CFire_Boob::Create(pBoob, m_pThis), CEngine::GetInstance()->GetCurSceneNumber());

	return S_OK;
}

void CFire_explosion::Update(_double dDeltaTime)
{
	if (m_DurationDelta > 0.3)
	{
		//m_pThis->SetActive(true);
		m_pEffectTrans->SetScale(_float3(m_fScale - 0.5f, m_fScale, 1.f));
		m_pChildren->SetScale(_float3(m_fScale, m_fScale, 1.f));
		m_fScale += dDeltaTime * 2.0;
	}
}

void CFire_explosion::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CFire_explosion::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CFire_explosion*		pInstance = new CFire_explosion();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CFire_explosion");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFire_explosion::Free()
{
}
