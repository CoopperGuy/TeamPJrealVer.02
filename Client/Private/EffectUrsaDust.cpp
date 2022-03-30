#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectUrsaDust.h"

USING(Client)

CEffectUrsaDust::CEffectUrsaDust()
{
}

CEffectUrsaDust * CEffectUrsaDust::Create(void * pArg, _matrix pos)
{
	CEffectUrsaDust*		pInstance = new CEffectUrsaDust();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectUrsaDust");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectUrsaDust::Initialize(void* pArg, _matrix pos)
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

void CEffectUrsaDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	_float3 m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) , m_pTransform->GetScale(CTransform::STATE_LOOK) };


	m_pTransform->SetScale(_float3(m_fScale.x += deltaTime, m_fScale.y += deltaTime, m_fScale.z += deltaTime));

}

void CEffectUrsaDust::LateUpdate(_double deltaTime)
{
	m_dDeadTime += deltaTime;

	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration() <= (_float)m_dDeadTime)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectUrsaDust::Render()
{
}


void CEffectUrsaDust::Free()
{
	__super::Free();
}
