#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectDropRockDust.h"
#include "EmptyEffect.h"

USING(Client)

CEffectDropRockDust::CEffectDropRockDust()
{
}

CEffectDropRockDust * CEffectDropRockDust::Create(void * pArg ,_vector pos)
{
	CEffectDropRockDust*		pInstance = new CEffectDropRockDust();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectDropRockDust");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectDropRockDust::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {


		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;


		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		pos = XMVectorSetY(pos, 0.3f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	}
	return S_OK;
}

void CEffectDropRockDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_fFrame += (_float)deltaTime;

}

void CEffectDropRockDust::LateUpdate(_double deltaTime)
{


	if (m_fFrame >= static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectDropRockDust::Render()
{
}

void CEffectDropRockDust::Free()
{
	__super::Free();
}
