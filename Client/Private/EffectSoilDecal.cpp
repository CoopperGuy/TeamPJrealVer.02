#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectSoilDecal.h"

USING(Client)

CEffectSoilDecal::CEffectSoilDecal()
{
}

CEffectSoilDecal * CEffectSoilDecal::Create(void * pArg, _vector pos)
{
	CEffectSoilDecal*		pInstance = new CEffectSoilDecal();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectSoilDecal");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectSoilDecal::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, 1.f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectSoilDecal::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;


}

void CEffectSoilDecal::LateUpdate(_double deltaTime)
{
	if (m_dDeadTime>=static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectSoilDecal::Render()
{
}


void CEffectSoilDecal::Free()
{
	__super::Free();
}
