#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectUrsaShoulder.h"
#include "EmptyEffect.h"

USING(Client)

CEffectUrsaShoulder::CEffectUrsaShoulder()
{
}

CEffectUrsaShoulder * CEffectUrsaShoulder::Create(void * pArg, _matrix pos)
{
	CEffectUrsaShoulder*		pInstance = new CEffectUrsaShoulder();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectUrsaShoulder");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectUrsaShoulder::Initialize(void* pArg, _matrix pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		m_pTransform->SetMatrix(pos);

	}
	return S_OK;
}

void CEffectUrsaShoulder::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	Deaddt += deltaTime;
}

void CEffectUrsaShoulder::LateUpdate(_double deltaTime)
{
	if (Deaddt >=static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectUrsaShoulder::Render()
{
}

void CEffectUrsaShoulder::Free()
{
	__super::Free();
}

void CEffectUrsaShoulder::Set_Matrix(_matrix pos)
{

	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	if(m_pTransform)
		m_pTransform->SetMatrix(pos);


}
