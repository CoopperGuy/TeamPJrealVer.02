#include "stdafx.h"
#include "..\Public\TestMonster.h"

USING(Client)

CTestMonster::CTestMonster()
{
}

CTestMonster::CTestMonster(CGameObject * pObj)
	:CEnemy(pObj)
{
	
}

HRESULT CTestMonster::Initialize()
{
	CEngine*	pEngine = CEngine::GetInstance();
	m_pThisObj = static_cast<CEmptyGameObject*>(pEngine->FindGameObjectWithName(pEngine->GetCurSceneNumber(), "TestMonster"));
	m_pThisModel = static_cast<CModel*>(m_pThisObj->GetComponent("Com_Model"));
	m_pThisStat = static_cast<CStat*>(m_pThisObj->GetComponent("Com_Stat"));
	m_pThisModel->SetAnimationLoop(0, true);
	return S_OK;
}

void CTestMonster::Update(_double deltaTime)
{
	m_pThisModel->Play_Animation(deltaTime);
	m_eCurSTATES = CStat::STATES_ATK;
}

void CTestMonster::LateUpdate(_double dDeltaTime)
{
	m_pThisStat->SetSTATE(m_eCurSTATES);
}

void CTestMonster::Render()
{
}

CTestMonster * CTestMonster::Create(CGameObject * pObj)
{
	CTestMonster*		pInstance = new CTestMonster(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CTestMonster");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTestMonster::Free()
{
}

