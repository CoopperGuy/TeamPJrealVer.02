#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectBlood.h"

USING(Client)

CEffectBlood::CEffectBlood()
{
}

CEffectBlood * CEffectBlood::Create(void * pArg, _vector pos)
{
	CEffectBlood*		pInstance = new CEffectBlood();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectBlood");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectBlood::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.2f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		Startscail.x = m_pTransform->GetScale(CTransform::STATE_RIGHT);
		Startscail.y = m_pTransform->GetScale(CTransform::STATE_UP);

	}
	return S_OK;
}

void CEffectBlood::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;

	Startscail.x += (_float)deltaTime*0.1;
	Startscail.y += (_float)deltaTime *0.1;


	m_pTransform->SetScale(_float3(Startscail));

	if (m_pTransform->GetScale(CTransform::STATE_UP) >= 1.3f)
		m_bDead = true;

}

void CEffectBlood::LateUpdate(_double deltaTime)
{
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectBlood::Render()
{
}


void CEffectBlood::Free()
{
	__super::Free();
}
