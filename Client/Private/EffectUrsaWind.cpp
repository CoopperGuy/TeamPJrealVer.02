#include "stdafx.h"
#include "..\Public\EffectUrsaWind.h"

USING(Client)

CEffectUrsaWind::CEffectUrsaWind()
{
}

HRESULT CEffectUrsaWind::Initialize(void * pArg, _vector pos)
{

	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, 0.4f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectUrsaWind::Update(_double deltaTime)
{
	m_DurationDelta += deltaTime;
}

void CEffectUrsaWind::LateUpdate(_double deltaTime)
{
	if (m_DurationDelta){
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectUrsaWind::SetPos(_vector pos)
{
	if (m_bDead)
		return;

	if (!m_pTransform)
		return;

	m_pTransform->SetState(CTransform::STATE_POSITION, pos);
}

CEffectUrsaWind * CEffectUrsaWind::Create(void * pArg, _vector pos)
{
	CEffectUrsaWind*		pInstance = new CEffectUrsaWind();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectUrsaWind");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffectUrsaWind::Free()
{
}
