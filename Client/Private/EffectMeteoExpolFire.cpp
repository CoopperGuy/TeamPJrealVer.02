#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMeteoExpolFire.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
USING(Client)

CEffectMeteoExpolFire::CEffectMeteoExpolFire()
{
}

CEffectMeteoExpolFire * CEffectMeteoExpolFire::Create(void * pArg, _vector pos)
{
	CEffectMeteoExpolFire*		pInstance = new CEffectMeteoExpolFire();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMeteoExpolFire");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMeteoExpolFire::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.2f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectMeteoExpolFire::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	m_dDeadTime += deltaTime;
		
}


void CEffectMeteoExpolFire::LateUpdate(_double deltaTime)
{
	if (m_dDeadTime >= static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMeteoExpolFire::Render()
{
}


void CEffectMeteoExpolFire::Free()
{
	__super::Free();
}
