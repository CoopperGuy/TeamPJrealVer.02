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
	Scenes = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Boss3"));
	m_pScenes.emplace_back(Scenes);

	return S_OK;
}

void CPortalUI::Update(_double deltaTime)
{
	m_pProtalUI->SetActive(m_bIsActive);
	m_pPosition->SetActive(m_bIsActive);
	if (m_bIsActive) {
		if (m_pLeftArrow->isFristEnter()) {
			CEngine::GetInstance()->StopSound(CHANNELID::UI02);
			CEngine::GetInstance()->PlaySoundW("UIHover.mp3", CHANNELID::UI02);
		}
		if (m_pRightArrow->isFristEnter()) {
			CEngine::GetInstance()->StopSound(CHANNELID::UI02);
			CEngine::GetInstance()->PlaySoundW("UIHover.mp3", CHANNELID::UI02);
		}
		if (m_pLeftArrow->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
				if (m_iMinIdx < m_iCurIndex) {
					m_fDestScnensPosX += WINCX >> 1;
					m_iCurIndex--;
					m_bMove = true;
				}
			}
		}
		if (m_pRightArrow->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
				CEngine::GetInstance()->StopSound(CHANNELID::UI02);
				CEngine::GetInstance()->PlaySoundW("PortalArrowSelect.mp3", CHANNELID::UI02);
				if (m_iMaxIdx > m_iCurIndex) {
					m_fDestScnensPosX -= WINCX >> 1;
					m_iCurIndex++;
					m_bMove = true;
				}
			}
		}
		_float t = _float(m_moveDelta / m_moveTime);
		if (t < 1.f && m_bMove) {
			m_moveDelta += deltaTime;
			m_fCurScnensPosX = m_fPreScnensPosX * (1 - t) + m_fDestScnensPosX * t;
		}
		else {
			m_bMove = false;
			m_fPreScnensPosX = m_fDestScnensPosX;
			m_moveDelta = 0;
			m_fCurScnensPosX = m_fPreScnensPosX * (1 - t) + m_fDestScnensPosX * t;
		}
		_float2 scenesPos = m_pPosition->GetPosition();
		m_pPosition->SetPosition(m_fCurScnensPosX, scenesPos.y);
		_uint i = 0; 
		for (auto& iter : m_pScenes) 
		{
			if (iter->IsHovered()) 
			{
				if (m_iCurIndex == i) 
				{
					if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) 
					{
						m_eScenes = SCENES(i + 1);
						ClosePortalUI();
						m_fCurScnensPosX = _float(WINCX>>1);
					}
				}
			}
			i++;
		}

		if (m_pEscape->isFristEnter()) {
			CEngine::GetInstance()->StopSound(CHANNELID::UI02);
			CEngine::GetInstance()->PlaySoundW("UIHover.mp3", CHANNELID::UI02);
		}
		if (m_pEscape->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON))
			{
				CEngine::GetInstance()->StopSound(CHANNELID::UI03);
				CEngine::GetInstance()->PlaySoundW("UIClose.mp3", CHANNELID::UI03);
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

