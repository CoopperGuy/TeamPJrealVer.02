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

	m_pModel->SetUp_AnimationIndex(IDLE);
	return S_OK;
}

void CTrapSpear::Update(_double dDeltaTime)
{
	m_pOBB->p_States = CBasicCollider::STATES::STATES_ATK;

	if (m_dDeltaTime > m_dStart)
		m_bStart = true;

	m_dDeltaTime += dDeltaTime;

	if (m_bStart)
	{
		m_eState = PRICK;
		m_bStart = false;
	}

	CheckingFinished();

	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
}

void CTrapSpear::LateUpdate(_double dDeltaTime)
{
	m_pModel->Play_Animation(dDeltaTime);
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
	if (m_pModel->Get_AnimIndex() != (_uint)IDLE)
		m_dDeltaTime = 0.0;

	if (m_pModel->Get_isFinished())
	{
		switch (m_eState)
		{
		case ATK:
			m_eState = GO_INTO;
			break;
		case PRICK:
			m_eState = ATK;
			break;
		case GO_INTO:
			m_eState = IDLE;
			m_dDeltaTime = 0.0;
			break;
		case IDLE:
			break;
		}
	}
}
