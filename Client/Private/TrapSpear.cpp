#include "stdafx.h"
#include "..\Public\TrapSpear.h"

#include "BasicCollider.h"

USING(Client)

CTrapSpear::CTrapSpear()
{
}

CTrapSpear::CTrapSpear(CGameObject * pObj)
	:CEnemy(pObj)
{
}

HRESULT CTrapSpear::Initialize(void* pArg, _double dStartTime)
{
	if(pArg)
		m_pGameObject = (CGameObject*)pArg;
	if (m_pGameObject == nullptr)
		return E_FAIL;
	m_dStart = dStartTime;
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	m_pOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	m_pModel->SetAnimationLoop((_uint)eTrapSpear::PRICK, false);
	m_pModel->SetAnimationLoop((_uint)eTrapSpear::GO_INTO, false);
	m_eState = eTrapSpear::IDLE;
	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	return S_OK;
}

void CTrapSpear::Update(_double dDeltaTime)
{
	m_pOBB->p_States = CBasicCollider::STATES::STATES_ATK;

	if (m_dDeltaTime > m_dStart)
	{
		if(State_IDLE())
			m_bStart = true;
	}
	m_dDeltaTime += dDeltaTime;

	if (m_bStart)
	{
		m_eState = eTrapSpear::PRICK;
		m_dAnimSpeed = 0.7;
		m_bStart = false;
	}

	CheckingFinished();

	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
}

void CTrapSpear::LateUpdate(_double dDeltaTime)
{
	m_pModel->Play_Animation(dDeltaTime * m_dAnimSpeed);
}

void CTrapSpear::Render()
{
}

CTrapSpear * CTrapSpear::Create(void* pArg, _double dStartTime)
{
	CTrapSpear*		pInstance = new CTrapSpear();

	if (FAILED(pInstance->Initialize(pArg, dStartTime)))
	{
		MSG_BOX("Failed to Create CTrapSpear");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTrapSpear::Free()
{
	__super::Free();
}

void CTrapSpear::CheckingFinished()
{
	if (m_pModel->Get_AnimIndex() != (_uint)eTrapSpear::IDLE)
		m_dDeltaTime = 0.0;

	if (m_pModel->Get_isFinished())
	{
		switch (m_eState)
		{
		case eTrapSpear::ATK:
			m_dAnimSpeed = 1.0;
			m_eState = eTrapSpear::GO_INTO;
			break;
		case eTrapSpear::PRICK:
			m_dAnimSpeed = 0.6;
			m_eState = eTrapSpear::ATK;
			break;
		case eTrapSpear::GO_INTO:
			m_eState = eTrapSpear::IDLE;
			m_dDeltaTime = 0.0;
			break;
		case eTrapSpear::IDLE:
			break;
		}
	}
}

_bool CTrapSpear::State_IDLE()
{
	if (m_pModel->Get_AnimIndex() == (_uint)eTrapSpear::IDLE)
		return true;
	else
		return false;
}
