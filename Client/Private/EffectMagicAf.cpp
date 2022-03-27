#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMagicAf.h"
#include "EmptyEffect.h"
USING(Client)

CEffectMagicAf::CEffectMagicAf()
{
}

CEffectMagicAf * CEffectMagicAf::Create(void * pArg, _vector pos)
{
	CEffectMagicAf*		pInstance = new CEffectMagicAf();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMagicAf");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMagicAf::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		pos = XMVectorSetY(pos, 0.1f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		
	}
	return S_OK;
}

void CEffectMagicAf::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	
	m_dFadeoutDu += deltaTime;

}


void CEffectMagicAf::LateUpdate(_double deltaTime)
{
	if (m_dFadeoutDu >= static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMagicAf::Render()
{
}


void CEffectMagicAf::Free()
{
	__super::Free();
}
