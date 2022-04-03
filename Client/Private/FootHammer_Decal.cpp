#include "stdafx.h"
#include "..\Public\FootHammer_Decal.h"
#include "EventCheck.h"
#include "FootHammer_Bomb.h"
USING(Client)

CFootHammer_Decal::CFootHammer_Decal()
{
}

HRESULT CFootHammer_Decal::Initialize(CEmptyEffect* pThis, CGameObject* pTarget,_uint iCount, _fvector vDist)
{
	m_pThis = pThis;
	CTransform* pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	_vector vTargetPos = pTargetTrans->GetState(CTransform::STATE_POSITION);
	vTargetPos += pTargetTrans->GetState(CTransform::STATE_LOOK) * (_float)iCount * 0.6f;
	
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);

	return S_OK;
}

void CFootHammer_Decal::Update(_double dDeltaTime)
{
	m_DurationDelta += (_float)dDeltaTime;

	if (m_DurationDelta < m_pThis->GetFadeInDuration())
	{
		m_pEffectTrans->SetUpRotation(m_pEffectTrans->GetState(CTransform::STATE_UP), m_fAngle);
		m_fAngle += (_float)dDeltaTime * 100.f;
	}
	else
	{
		if (m_bCreateEffect)
		{

			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_FootHamer_Bomb", "E_FootHamer_Bomb");
			CEngine::GetInstance()->AddScriptObject(CFootHammer_Bomb::Create((CEmptyEffect*)pGameObject, m_pThis), CEngine::GetInstance()->GetCurSceneNumber());
			CEngine::GetInstance()->AddScriptObject(CFootHammer_Bomb::Create((CEmptyEffect*)pGameObject, m_pThis), CEngine::GetInstance()->GetCurSceneNumber());
			CEngine::GetInstance()->StopSound(ENEMY20);
			CEngine::GetInstance()->StopSound(ENEMY21);
			CEngine::GetInstance()->PlaySoundW("FootHammer_Bomb.mp3", ENEMY20);
			CEngine::GetInstance()->PlaySoundW("BombSub_Flogas.mp3", ENEMY21);
			m_bCreateEffect = false;
		}
	}
}

void CFootHammer_Decal::LateUpdate(_double dDeltaTime)
{
	if (m_DurationDelta >= m_pThis->GetFadeOutDuration())
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CFootHammer_Decal::Create(CEmptyEffect* pThis, CGameObject* pTarget, _uint iCount, _fvector vDist)
{
	CFootHammer_Decal*		pInstance = new CFootHammer_Decal();

	if (FAILED(pInstance->Initialize(pThis, pTarget, iCount, vDist)))
	{
		MSG_BOX("Failed to Create CFootHammer_Decal");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFootHammer_Decal::Free()
{
}
