#include "stdafx.h"
#include "..\Public\PortalUI.h"

USING(Client)

CPortalUI::CPortalUI()
{
}

HRESULT CPortalUI::Initailze(CGameObject * pArg)
{
	m_pProtalUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "PortalUI"));
	m_pLeftArrow = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "LeftArrow"));
	m_pRightArrow = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "LeftArrow"));
	m_pEscape = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "EscapePortal"));

	m_pPosition = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "ScenePosition"));
	m_pScenes = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Boss1"));
	
	return S_OK;
}

void CPortalUI::Update(_double deltaTime)
{
	m_pProtalUI->SetActive(m_bIsActive);
	m_pPosition->SetActive(m_bIsActive);
	if (m_bIsActive) {
		if (m_pScenes->IsHovered()) {
			if (CEngine::GetInstance()->IsMouseDown(0)) {
				m_eScenes = SCENES::STAGE02;

				ClosePortalUI();
			}
		}
		if (m_pEscape->IsHovered()) {
			if (CEngine::GetInstance()->IsMouseDown(0)) {
				ClosePortalUI();
			}
		}
	}
}

void CPortalUI::LateUpdate(_double deltaTime)
{
}

void CPortalUI::Render()
{
}

void CPortalUI::ClosePortalUI()
{
	m_bIsActive = false;
	m_pProtalUI->SetActive(m_bIsActive);
	m_pPosition->SetActive(m_bIsActive);
	g_AnotherMenu = false;
}

CPortalUI * CPortalUI::Create(CGameObject * pTarget)
{
	CPortalUI*	obj = new CPortalUI();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CPortalUI::Free()
{
}

