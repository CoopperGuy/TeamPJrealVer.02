#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectBloodDecal.h"

USING(Client)

CEffectBloodDecal::CEffectBloodDecal()
{
}

CEffectBloodDecal * CEffectBloodDecal::Create(void * pArg, _vector pos)
{
	CEffectBloodDecal*		pInstance = new CEffectBloodDecal();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectBloodDecal");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectBloodDecal::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

	}
	return S_OK;
}

void CEffectBloodDecal::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;

	if (m_pTransform->GetScale(CTransform::STATE_UP) >= 1.3f)
		m_bDead = true;

}

void CEffectBloodDecal::LateUpdate(_double deltaTime)
{
	if (m_dDeadTime>=10.f)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectBloodDecal::Render()
{
}


void CEffectBloodDecal::Free()
{
	__super::Free();
}
