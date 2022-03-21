#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectFireBall.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectFireBall::CEffectFireBall()
{
}

CEffectFireBall * CEffectFireBall::Create(void * pArg, _vector pos)
{
	CEffectFireBall*		pInstance = new CEffectFireBall();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectFireBall");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectFireBall::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		_vector pPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectFireBall::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
		m_bDead = true;
}


void CEffectFireBall::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectFireBall::Render()
{
}


void CEffectFireBall::Free()
{
	__super::Free();
}

void CEffectFireBall::SetPos(_matrix _pmatrix)
{
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetMatrix(_pmatrix);
	}
}

void CEffectFireBall::Set_Pos(_vector pPos)
{
}
