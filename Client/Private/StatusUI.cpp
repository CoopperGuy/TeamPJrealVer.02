#include "stdafx.h"
#include "..\Public\StatusUI.h"
#include "EventCheck.h"
USING(Client)

CStatusUI::CStatusUI()
{
}

HRESULT CStatusUI::Initailze(CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();

	m_pPlayerStatus = static_cast<CStat*>(pEngine->FindGameObjectWithName(0, "Player")->GetComponent("Com_Stat"));
	m_vecTexts.reserve(STATUS_END);
	CGameObject* texts = pEngine->FindGameObjectWithName(0, "Texts");
	list<CGameObject*> childs = texts->GetChildren();
	_int i = 0;
	for (auto& iter : childs) {
		if (i > 6) {
			m_vecTexts.emplace_back(static_cast<CText*>(iter->GetComponent("Com_Text")));
		}
		i++;
	}
	return S_OK;
}

void CStatusUI::Update(_double deltaTime)
{
	CStat::STAT _playerStat = m_pPlayerStatus->GetStatInfo();
	m_vecTexts[STATUS_LEVEL]->SetString(to_string(_playerStat.level));
	string strText = to_string((_int)_playerStat.hp) + "/" + to_string((_int)_playerStat.maxHp);
	m_vecTexts[STATUS_HP]->SetString(strText);
	m_vecTexts[STATUS_ATK]->SetString(to_string((_int)_playerStat.atk));
	m_vecTexts[STATUS_DEF]->SetString(to_string((_int)_playerStat.armor));
	m_vecTexts[STATUS_CRIT]->SetString(to_string((_int)(_playerStat.critical *  100.f)));
	strText = to_string((_int)(m_pPlayerStatus->GetStaPercentage() * 100.f)) + "/100";
	m_vecTexts[STATUS_STA]->SetString(strText);
	strText = to_string((_int)_playerStat.exp) + "/" + to_string((_int)_playerStat.maxExp);
	m_vecTexts[STATUS_EXP]->SetString(strText);
	m_vecTexts[STATUS_BOHP]->SetString("+" + to_string((_int)_playerStat.boHp));
	m_vecTexts[STATUS_HOATK]->SetString("+" + to_string((_int)_playerStat.boAtk));
	m_vecTexts[STATUS_BODEF]->SetString("+" + to_string((_int)_playerStat.boArmor));

}

void CStatusUI::LateUpdate(_double deltaTime)
{
}

void CStatusUI::Render()
{
}

CStatusUI * CStatusUI::Create(CGameObject * pTarget)
{
	CStatusUI*	obj = new CStatusUI();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CStatusUI::Free()
{
}

