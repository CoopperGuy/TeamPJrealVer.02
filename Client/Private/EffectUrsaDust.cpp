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

		//_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);

		//int random = rand() % 3;
		//random += 1;
		//random = random * 0.3f;

		//int ranX = rand() % 2;
		//if (ranX == 0)
		//	pos = XMVectorSetX(pos, XMVectorGetX(pos) + random);
		//else
		//	pos = XMVectorSetX(pos, XMVectorGetX(pos) - random);

		//pos = XMVectorSetY(pos, XMVectorGetY(pos) + random);

		//m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	}
	return S_OK;
}

void CEffectUrsaDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	//_float3 m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) , m_pTransform->GetScale(CTransform::STATE_LOOK) };


	//m_pTransform->SetScale(_float3(m_fScale.x += 0.05f, m_fScale.y, m_fScale.z));

}

void CEffectUrsaDust::LateUpdate(_double deltaTime)
{
	m_dDeadTime += deltaTime;

	// (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration() <= (_float)m_dDeadTime)
	//if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
	if (m_dDeadTime>=0.5f)
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
