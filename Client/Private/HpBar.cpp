#include "stdafx.h"
#include "..\Public\HpBar.h"
#include "Stat.h"
USING(Client)

CHpBar::CHpBar()
{
}

HRESULT CHpBar::Initailze(CGameObject * pArg)
{	
	//pArg for Player
	if (pArg != nullptr) {
		pTarget = pArg;
		m_pTargetStat =static_cast<CStat*>(pTarget->GetComponent("Com_Stat"));
	}
	m_pHpBar = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "HpBar"));
	m_pSTABar = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "StaminaBar"));
	m_pExpBar = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "ExpBar"));

	return S_OK;
}

void CHpBar::Update(_double deltaTime)
{
}

void CHpBar::LateUpdate(_double deltaTime)
{
	//use pTarget hp
	if (m_pTargetStat)
		m_fPercetage = m_pTargetStat->GetHpPercentage();
	if (m_fPercetage < m_fBackPercentage) 
		m_fBackPercentage -= _float (0.05f*deltaTime);
	
	m_pHpBar->SetPercentage(m_fPercetage);
	m_pHpBar->SetBackPercetage(m_fBackPercentage);
	//have to change color
	if (m_pTargetStat)
		m_fPercetage = m_pTargetStat->GetStaPercentage();
	if (m_fPercetage < m_fBackPercentageSta) 
		m_fBackPercentageSta -= _float(0.05f*deltaTime);
	
	m_pSTABar->SetPercentage(m_fPercetage);
	m_pSTABar->SetBackPercetage(m_fBackPercentageSta);

	//
	if (m_pTargetStat)
		m_fPercetage = m_pTargetStat->GetExpPercentage();
	if (m_fPercetage < m_fBackPercentageExp) 
		m_fBackPercentageExp -= _float(0.05f*deltaTime);
	
	m_pExpBar->SetPercentage(m_fPercetage);
	m_pExpBar->SetBackPercetage(m_fBackPercentageExp);

}

void CHpBar::Render()
{
}

CHpBar * CHpBar::Create(CGameObject * pArg)
{
	CHpBar*	obj = new CHpBar();
	if (FAILED(obj->Initailze(pArg))) {
		SafeRelease(obj);
		return nullptr;
	}
	return obj;
}

void CHpBar::Free()
{

}


