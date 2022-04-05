#include "stdafx.h"
#include "..\Public\MenuHud.h"
#include "BackPackHud.h"
#include "QuestHud.h"
#include "EventCheck.h"
#include "StatusUI.h"
#include "SoundHud.h"
USING(Client)

CMenuHud::CMenuHud()
{
}

HRESULT CMenuHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
	}
	m_pThisUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "EscMenu"));
	list<CGameObject*> child = m_pThisUI->GetChildren();
	for (auto& iter : child) {
		m_pMenuButton.emplace_back(dynamic_cast<CEmptyUI*>(iter));
	}
	m_pThisUI->SetActive(false);
	CEventCheck::GetInstance()->SetMenus(this);
	return S_OK;
}

void CMenuHud::Update(_double deltaTime)
{
	if (CEngine::GetInstance()->IsKeyDown(VK_ESCAPE)) {
		if (!g_AnotherMenu) {
			m_bisActive = !m_bisActive;
			m_pThisUI->SetActive(m_bisActive);
			_uint idx = 0;
			_bool isChildMenu = false;
			for (auto& iter : m_pMenuButton) {
				iter->SetIsSelected(false);
				if (m_pMenuList[idx] != nullptr) {
					m_pMenuList[idx]->SetActive(false);
				}
				idx++;
			}
			g_Menu = m_bisActive;
		}
	}
	for (auto& iter : m_pMenuButton) {
		if (iter->isFristEnter()) {
			CEngine::GetInstance()->StopSound(CHANNELID::UI00);
			CEngine::GetInstance()->PlaySoundW("CURSOL_CLICK.wav", CHANNELID::UI00);
		}
		if (iter->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON))
			{
				CEngine::GetInstance()->StopSound(CHANNELID::UI01);
				CEngine::GetInstance()->PlaySoundW("EscMenuselect.mp3", CHANNELID::UI01);
			}
		}
	}
	_uint i = 0;
	for (auto& iter : m_pMenuButton) {
		if (m_pMenuList[i] == nullptr) {
			CEmptyUI*	emptyUI = nullptr;
			switch (i)
			{
			case Client::CMenuHud::MENU_EQUIP:
				emptyUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "Backpack_Hud"));
				m_pBackPack = CBackPackHud::Create();
				CEngine::GetInstance()->AddScriptObject(m_pBackPack, 0);
				m_pBackPack->LinkInventoryToHud(m_pInven);
				break;
			case Client::CMenuHud::MENU_QUEST:
				emptyUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "Quest_Hud"));
				m_pQuestHud = CQuestHud::Create();
				CEngine::GetInstance()->AddScriptObject(m_pQuestHud, 0);
				break;
			case Client::CMenuHud::MENU_STATUS:
				emptyUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "StatusUI"));
				m_pStatus = CStatusUI::Create();
				CEngine::GetInstance()->AddScriptObject(m_pStatus, 0);
				break;
			case Client::CMenuHud::MENU_SOUND:
				emptyUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "SoundUI"));
				m_pSound = CSoundHud::Create();
				CEngine::GetInstance()->AddScriptObject(m_pSound, 0);
				break;
			case Client::CMenuHud::MENU_END:
				break;
			}
			m_pMenuList[i] = emptyUI;
			continue;
		}
		if (iter->IsSelected()) {
			m_bisActive = false;
			m_pThisUI->SetActive(m_bisActive);
			m_pMenuList[i]->SetActive(true);
		}
		i++;
	}
	if (m_pMenuList[Client::CMenuHud::MENU_EQUIP] != nullptr) {
		if (!m_bRightOpenInven) {
			_float2 pos = m_pMenuList[Client::CMenuHud::MENU_EQUIP]->GetPosition();
			m_pMenuList[Client::CMenuHud::MENU_EQUIP]->SetPosition(302.f , pos.y);
		}
		else {
			_float2 pos = m_pMenuList[Client::CMenuHud::MENU_EQUIP]->GetPosition();
			m_pMenuList[Client::CMenuHud::MENU_EQUIP]->SetPosition(987, pos.y);
			m_pMenuList[Client::CMenuHud::MENU_EQUIP]->SetActive(true);
		}
		m_pBackPack->SetRightLeft(m_bRightOpenInven);
	}

}

void CMenuHud::LateUpdate(_double deltaTime)
{
}

void CMenuHud::Render()
{
}

void CMenuHud::AddInvenInBackPackHud(CInventory * inven)
{
}

void CMenuHud::LinkInventoryToHud(CInventory * inven)
{
	if(inven != nullptr)
		m_pInven = inven;
}

void CMenuHud::OffAllMenus()
{
	g_Menu = false;
	g_AnotherMenu = false;
	m_bisActive = false;
	m_pThisUI->SetActive(false);
	for (int i = 0; i < (_int)MENU_END; i++) {
		m_pMenuList[i]->SetActive(false);
	}
}

CMenuHud * CMenuHud::Create(CGameObject * pTarget)
{
	CMenuHud*	obj = new CMenuHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CMenuHud::Free()
{
}


