#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectDropRock.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectDropRock::CEffectDropRock()
{
}

CEffectDropRock * CEffectDropRock::Create(void * pArg, _vector pos)
{
	CEffectDropRock*		pInstance = new CEffectDropRock();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectDropRock");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectDropRock::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		_vector pPos = m_pTransform->GetState(CTransform::STATE_POSITION);


		pos = XMVectorSetY(pos, XMVectorGetY(pPos) - 0.09f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		//static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeInEnable(true);
		//static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeOutStartTime(0.f);
		//static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeOutDuration(1.3f);
		//static_cast<CEmptyEffect*>(m_pGameObject)->SetEffectDuration(1.3f);


	}
	return S_OK;
}

void CEffectDropRock::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	if (0.9f >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
		deaddt += deltaTime;
	}

	//if(static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutEnable())
	//	deaddt += deltaTime;


}


void CEffectDropRock::LateUpdate(_double deltaTime)
{

	if (deaddt >= static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectDropRock::Render()
{
}


void CEffectDropRock::Free()
{
	__super::Free();
}

void CEffectDropRock::SetPos(_matrix _pmatrix)
{
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetMatrix(_pmatrix);
	}
}

void CEffectDropRock::Set_Pos(_vector pPos)
{
	m_pTransform->SetState(CTransform::STATE_POSITION, pPos);
}
