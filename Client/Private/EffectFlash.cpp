#include "stdafx.h"
#include "..\Public\EffectFlash.h"
#include "SpriteFire.h"
#include "El_Flogas.h"

USING(Client)

CEffectFlash::CEffectFlash()
{
}

HRESULT CEffectFlash::Initialize(CEmptyEffect* pThis, CGameObject* pTarget, CEl_Flogas& pEl)
{
	
	m_pThis = pThis;
	m_pEl = &pEl;
	CTransform* pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	_vector vTargetPos = pTargetTrans->GetState(CTransform::STATE_POSITION);
	vTargetPos += m_pEl->Get_EffectDist() * (_float)m_pEl->Get_Count() * 0.1f;
	vTargetPos = XMVectorSetY(vTargetPos, 0.5f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	m_fScale = m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);

	return S_OK;
}

void CEffectFlash::Update(_double dDeltaTime)
{
	m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, 1.f));
	m_fScale += (_float)dDeltaTime;
}

void CEffectFlash::LateUpdate(_double deltaTime)
{
	m_DurationDelta += (_float)deltaTime;
	if (m_DurationDelta > m_pThis->GetFadeOutDuration()) 
	{
		CEngine::GetInstance()->StopSound(ENEMY20);
		CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_E_Sprite_Fire", "E_Sprite_Fire");
		CEngine::GetInstance()->AddScriptObject(CSpriteFire::Create((CEmptyEffect*)pGameObject, m_pThis, m_pEl), CEngine::GetInstance()->GetCurSceneNumber());
		CEngine::GetInstance()->PlaySoundW("Explosion_Element.mp3", ENEMY20);
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CEffectFlash::Create(CEmptyEffect* pThis, CGameObject* pTarget, CEl_Flogas& pEl)
{
	CEffectFlash*		pInstance = new CEffectFlash();

	if (FAILED(pInstance->Initialize(pThis, pTarget, pEl)))
	{
		MSG_BOX("Failed to Create CEffectFlash");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffectFlash::Free()
{
}
