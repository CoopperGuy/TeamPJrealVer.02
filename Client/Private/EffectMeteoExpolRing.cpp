#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMeteoExpolRing.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
USING(Client)

CEffectMeteoExpolRing::CEffectMeteoExpolRing()
{
}

CEffectMeteoExpolRing * CEffectMeteoExpolRing::Create(void * pArg, _vector pos)
{
	CEffectMeteoExpolRing*		pInstance = new CEffectMeteoExpolRing();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMeteoExpolRing");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMeteoExpolRing::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		Startscail.x = m_pTransform->GetScale(CTransform::STATE_RIGHT);
		Startscail.y = m_pTransform->GetScale(CTransform::STATE_UP);

	}
	return S_OK;
}

void CEffectMeteoExpolRing::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	
	m_dDeadTime += deltaTime;

	Startscail.x += deltaTime;
	Startscail.y+= deltaTime;


	m_pTransform->SetScale(_float3(Startscail));

	if (m_pTransform->GetScale(CTransform::STATE_RIGHT) >= 1.5f)
		m_bDead = true;
}


void CEffectMeteoExpolRing::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMeteoExpolRing::Render()
{
}


void CEffectMeteoExpolRing::Free()
{
	__super::Free();
}
