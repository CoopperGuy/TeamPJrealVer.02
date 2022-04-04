#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectRing.h"
#include "EmptyEffect.h"
#include "Obb.h"


USING(Client)
CEffectRing::CEffectRing()
{
}
CEffectRing * CEffectRing::Create(void * pArg,_vector pos)
{
	CEffectRing*		pInstance = new CEffectRing();

	if (FAILED(pInstance->Initialize(pArg,pos)))
	{
		MSG_BOX("Failed to Create CEffectRing");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectRing::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		//pos = XMVectorSetY(pos, 0.1f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);


		_float3 tempscail = {0.1f, m_pTransform->GetScale(CTransform::STATE_UP), 0.1f};
		m_pTransform->SetScale(tempscail);

		StartScail = { m_pTransform->GetScale(CTransform::STATE_RIGHT),m_pTransform->GetScale(CTransform::STATE_UP),m_pTransform->GetScale(CTransform::STATE_LOOK) };
	}
	return S_OK;
}

void CEffectRing::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	StartScail.x += 0.3f;
	StartScail.z += 0.3f;

	m_pTransform->SetScale(StartScail);

}

void CEffectRing::LateUpdate(_double deltaTime)
{
	m_dDeadTime += deltaTime;
	if (m_dDeadTime>static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectRing::Render()
{
}


void CEffectRing::Free()
{
	__super::Free();
}
