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
	m_pRightArrow = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "RightArrow"));
	m_pEscape = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "EscapePortal"));

	m_pPosition = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "ScenePosition"));
	CEmptyUI* Scenes = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Boss1"));
	m_pScenes.emplace_back(Scenes);
	Scenes = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Boss2"));
	m_pScenes.emplace_back(Scenes);

	return S_OK;
}

void CPortalUI::Update(_double deltaTime)
{
	m_pProtalUI->SetActive(m_bIsActive);
	m_pPosition->SetActive(m_bIsActive);
	if (m_bIsActive) {
		if (m_pLeftArrow->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
				if (m_iMinIdx < m_iCurIndex) {
					m_fDestScnensPosX += 640.f;
					m_iCurIndex--;
				}
			}
		}
		if (m_pRightArrow->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
				if (m_iMaxIdx > m_iCurIndex) {
					m_fDestScnensPosX -= 640.f;
					m_iCurIndex++;
				}
			}
		}
		if ((m_fDestScnensPosX - m_fCurScnensPosX) >= 0.01f) {
			m_fCurScnensPosX += 640.f * (_float)deltaTime * 10.f;
		}
		if ((m_fCurScnensPosX - m_fDestScnensPosX) >= 0.01f) {
			m_fCurScnensPosX -= 640.f * (_float)deltaTime * 10.f;
		}
		_float2 scenesPos = m_pPosition->GetPosition();
		m_pPosition->SetPosition(m_fCurScnensPosX, scenesPos.y);
		_uint i = 0; 
		for (auto& iter : m_pScenes) {
			if (iter->IsHovered()) {
				if (m_iCurIndex == i) {
					if (CEngine::GetInstance()->IsMouseDown(0)) {
						m_eScenes = SCENES(i + 1);
						ClosePortalUI();
						m_fCurScnensPosX = 640.f;
					}
				}
			}
			i++;
		}

		if (m_pEscape->IsHovered()) {
			if (CEngine::GetInstance()->IsMouseDown(0)) {
				ClosePortalUI();
				m_fCurScnensPosX = 640.f;
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

