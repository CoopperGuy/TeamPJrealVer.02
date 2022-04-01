#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectUrsaDust.h"

USING(Client)

CEffectUrsaDust::CEffectUrsaDust()
{
}

CEffectUrsaDust * CEffectUrsaDust::Create(void * pArg)
{
	CEffectUrsaDust*		pInstance = new CEffectUrsaDust();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectUrsaDust");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

CEffectUrsaDust * CEffectUrsaDust::Create(void * pArg, _vector pos)
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


HRESULT CEffectUrsaDust::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		m_pComponent = m_pGameObject->GetComponent("Com_RectInstance");
		////m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		//m_pTransform->SetMatrix(pos);

		//_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);

		//int random = rand() % 3;
		//random += 1;
		//random = random * 0.37f;

		//int ranX = rand() % 2;
		//if (ranX == 0) {
		//	pos = XMVectorSetX(pos, XMVectorGetX(pos) + random);
		//	pos = XMVectorSetZ(pos, XMVectorGetZ(pos) + random);

		//}
		//else {
		//	pos = XMVectorSetX(pos, XMVectorGetX(pos) - random);
		//	pos = XMVectorSetZ(pos, XMVectorGetZ(pos) - random);
		//}
		//m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	}
	return S_OK;
}
HRESULT CEffectUrsaDust::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		//m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);

		//int random = rand() % 3;
		//random += 1;
		//random = random * 0.37f;

		//int ranX = rand() % 2;
		//if (ranX == 0) {
		//	pos = XMVectorSetX(pos, XMVectorGetX(pos) + random);
		//	pos = XMVectorSetZ(pos, XMVectorGetZ(pos) + random);

		//}
		//else {
		//	pos = XMVectorSetX(pos, XMVectorGetX(pos) - random);
		//	pos = XMVectorSetZ(pos, XMVectorGetZ(pos) - random);
		//}

		pos = XMVectorSetY(pos, 0.25f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
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
	m_dDeadTime += deltaTime;

}

void CEffectUrsaDust::LateUpdate(_double deltaTime)
{

	// (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration() <= (_float)m_dDeadTime)
	//if (m_dDeadTime>=0.5f)
	//if (static_cast<CVIBuffer_RectInstance*>(m_pComponent)->Get_LifeTime() <= m_dDeadTime)
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
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
