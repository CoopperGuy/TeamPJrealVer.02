#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectEAFire.h"
#include "EmptyEffect.h"
#include "Obb.h"
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
		m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) , 0.f };
	}
	return S_OK;
}

void CEffectEAFire::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	if (m_bSet) {

		if (m_bSetFadeOut)
		{
			SetFadeInOut(m_pGameObject);
			m_bSetFadeOut = false;
		}

		if (static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutEnable()) {
			m_fDeadTime += deltaTime;
			if (m_fDeadTime >= static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutDuration())
			{
				m_bDead = true;
			}
		}
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

void CEffectEAFire::SetFadeInOut(CGameObject * pObj)
{
	//static_cast<CEmptyEffect*>(pObj)->SetFadeInEnable(true);
	//static_cast<CEmptyEffect*>(pObj)->SetFadeInStartTime(0.f);
	//static_cast<CEmptyEffect*>(pObj)->SetFadeInDuration(0.5f);

	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(0.3f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(0.3f);
}

void CEffectEAFire::Free()
{
	__super::Free();
}
