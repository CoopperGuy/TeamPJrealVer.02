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

HRESULT CTrapSpear::Initialize(void* pArg, _double dEndTime)
{
	if(pArg)
		m_pGameObject = (CGameObject*)pArg;
	if (m_pGameObject == nullptr)
		return E_FAIL;
	m_dEnd = dEndTime;
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	m_pOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));
	m_pOBB1 = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB1"));
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	
	m_pModel->SetAnimationLoop((_uint)eTrapSpear::PRICK, false);
	m_pModel->SetAnimationLoop((_uint)eTrapSpear::ATK, false);
	m_pModel->SetAnimationLoop((_uint)eTrapSpear::GO_INTO, false);
	m_eState = eTrapSpear::IDLE;
	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	return S_OK;
}

void CTrapSpear::Update(_double dDeltaTime)
{
	m_pOBB->p_States = CBasicCollider::STATES::STATES_ATK;
	m_pOBB1->p_States = CBasicCollider::STATES::STATES_ATK;

	if (m_dStartTime > m_dStart)
	{
		if(State_IDLE())
			m_bStart = true;
	}

	m_dStartTime += dDeltaTime;
	
	if (m_bEnd)
		m_dEndTime += dDeltaTime;

	else if (m_bStart)
	{
		m_eState = eTrapSpear::PRICK;
		m_dAnimSpeed = 0.3;
		m_dStartTime = 0.0;
		m_bEnd = true;
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

CTrapSpear * CTrapSpear::Create(void* pArg, _double dEndTime)
{
	CTrapSpear*		pInstance = new CTrapSpear();

	if (FAILED(pInstance->Initialize(pArg, dEndTime)))
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
	if (m_pModel->Get_isFinished())
	{
		switch (m_eState)
		{
		case eTrapSpear::ATK:
			if (m_dEndTime > m_dEnd)
			{
				m_eState = eTrapSpear::GO_INTO;
				m_bEnd = false;
				m_dEndTime = 0.0;
				m_dAnimSpeed = 0.3;
			}
			break;
		case eTrapSpear::PRICK:
			m_dAnimSpeed = 1.5;
			m_eState = eTrapSpear::ATK;
			break;
		case eTrapSpear::GO_INTO:
			m_dAnimSpeed = 1.0;
			m_eState = eTrapSpear::IDLE;
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
