#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectUrsaPajang.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
#include "MeteoFireBall.h"
USING(Client)

CEffectUrsaPajang::CEffectUrsaPajang()
{
}

CEffectUrsaPajang * CEffectUrsaPajang::Create(void * pArg, _vector pos)
{
	CEffectUrsaPajang*		pInstance = new CEffectUrsaPajang();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectUrsaPajang");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectUrsaPajang::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		//pos = XMVectorSetY(pos, 0.1f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		
		StartScail = { m_pTransform->GetScale(CTransform::STATE_RIGHT),m_pTransform->GetScale(CTransform::STATE_UP),m_pTransform->GetScale(CTransform::STATE_LOOK) };

	}
	return S_OK;
}

void CEffectUrsaPajang::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	StartScail.x += 0.1f;
	StartScail.z += 0.1f;

	m_pTransform->SetScale(StartScail);
}


void CEffectUrsaPajang::LateUpdate(_double deltaTime)
{
	m_dDeadTime += deltaTime;
	if (m_dDeadTime>=0.8f)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectUrsaPajang::Render()
{
}


void CEffectUrsaPajang::Free()
{
	__super::Free();
}
