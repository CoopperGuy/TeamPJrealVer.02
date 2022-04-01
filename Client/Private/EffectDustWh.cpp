#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectDustWh.h"

USING(Client)

CEffectDustWh::CEffectDustWh()
{
}

CEffectDustWh * CEffectDustWh::Create(void * pArg, _matrix pos)
{
	CEffectDustWh*		pInstance = new CEffectDustWh();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectDustWh");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectDustWh::Initialize(void* pArg, _matrix pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		m_pTransform->SetMatrix(pos);

		//m_pTransform->SetScale(_float3(1.f, 2.f,1.f));


		//MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		//MyPos = XMVectorSetY(MyPos, 0.8f);

		//m_pTransform->SetState(CTransform::STATE_POSITION, MyPos);

	}
	return S_OK;
}

void CEffectDustWh::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

}

void CEffectDustWh::LateUpdate(_double deltaTime)
{
	m_dDeadTime += deltaTime;

	if (m_dDeadTime>=0.5f)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectDustWh::Render()
{
}


void CEffectDustWh::Free()
{
	__super::Free();
}


