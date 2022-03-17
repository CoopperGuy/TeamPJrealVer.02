#include "stdafx.h"
#include "..\Public\LevelFlag.h"
#include "Stat.h"
USING(Client)

LevelFlag::LevelFlag()
{
}

LevelFlag * LevelFlag::Create(CGameObject * pArg)
{
	LevelFlag*	obj = new LevelFlag();
	if (FAILED(obj->Initailze(pArg))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}


HRESULT LevelFlag::Initailze(CGameObject * pArg)
{
	//pArg for Player.
	if (pArg != nullptr) {
		pTarget = pArg;
		m_pTargetStat =static_cast<CStat*>(pTarget->GetComponent("Com_Stat"));
	}
	levelFlag = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "LevelFont");
	levelFlag20 = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "LevelFont20");
	levelFlag21 = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "LevelFont21");

	m_pVIBuffer = dynamic_cast<CVIBuffer_RectUI*>(levelFlag->GetComponent("Com_VIBuffer"));
	m_pVIBuffer20 = dynamic_cast<CVIBuffer_RectUI*>(levelFlag20->GetComponent("Com_VIBuffer"));
	m_pVIBuffer21 = dynamic_cast<CVIBuffer_RectUI*>(levelFlag21->GetComponent("Com_VIBuffer"));
	return S_OK;
}

void LevelFlag::Update(_double deltaTime)
{
}

void LevelFlag::LateUpdate(_double deltaTime)
{
	//use pTarget level
	if(m_pTargetStat)
		m_nextLevel = m_pTargetStat->GetStatInfo().level;
	if (m_curLevel != m_nextLevel) {
		m_curLevel = m_nextLevel;
		if (m_curLevel < 10) {
			levelFont = "../../Assets/UITexture/Font/DmgF_Null_" + to_string(m_curLevel) + ".dds";
			m_pVIBuffer->UpdateTexture(levelFont, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			levelFlag->SetActive(true);
			levelFlag20->SetActive(false);
			levelFlag21->SetActive(false);
		}
		else {
			levelFont = "../../Assets/UITexture/Font/DmgF_Null_" + to_string(m_curLevel/10) + ".dds";
			m_pVIBuffer20->UpdateTexture(levelFont, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			levelFont = "../../Assets/UITexture/Font/DmgF_Null_" + to_string(m_curLevel%10) + ".dds";
			m_pVIBuffer21->UpdateTexture(levelFont, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			levelFlag->SetActive(false);
			levelFlag20->SetActive(true);
			levelFlag21->SetActive(true);
		}
	}
}

void LevelFlag::Render()
{
}


void LevelFlag::Free()
{

}
