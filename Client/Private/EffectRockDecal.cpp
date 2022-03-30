#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectRockDecal.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
#include "MeteoFireBall.h"
USING(Client)

CEffectRockDecal::CEffectRockDecal()
{
}

CEffectRockDecal * CEffectRockDecal::Create(void * pArg, _vector pos)
{
	CEffectRockDecal*		pInstance = new CEffectRockDecal();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectRockDecal");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectRockDecal::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, 0.02f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectRockDecal::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	
	m_dDeadTime += deltaTime;
}


void CEffectRockDecal::LateUpdate(_double deltaTime)
{
	if (m_dDeadTime >= static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{

		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectRockDecal::Render()
{
}


void CEffectRockDecal::Free()
{
	__super::Free();
}
