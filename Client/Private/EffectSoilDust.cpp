#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectSoilDust.h"

USING(Client)

CEffectSoilDust::CEffectSoilDust()
{
}

CEffectSoilDust * CEffectSoilDust::Create(void * pArg, _matrix pos)
{
	CEffectSoilDust*		pInstance = new CEffectSoilDust();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectSoilDust");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectSoilDust::Initialize(void* pArg, _matrix pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		m_pTransform->SetMatrix(pos);

		m_pTransform->SetScale(_float3(2.f, 4.f,2.f));
	}
	return S_OK;
}

void CEffectSoilDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

}

void CEffectSoilDust::LateUpdate(_double deltaTime)
{
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectSoilDust::Render()
{
}


void CEffectSoilDust::Free()
{
	__super::Free();
}
