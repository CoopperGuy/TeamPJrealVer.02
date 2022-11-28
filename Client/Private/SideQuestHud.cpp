#include "stdafx.h"
#include "..\Public\SideQuestHud.h"

BEGIN(Client)
void ThreadUpate(CSideQuestHud* thisUI, std::mutex& m) {
	lock_guard<std::mutex> lock(m);
	thisUI->UpdateList();
}
END

USING(Client)

CSideQuestHud::CSideQuestHud()
{
}

HRESULT CSideQuestHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
	}
	m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "QuestListSub"));
	list<CGameObject*>	child = m_pThisUI->GetChildren();
	for (auto& iter : child) {
		m_pQuestSubList.emplace_back(static_cast<CEmptyUI*>(iter));
	}
	CEngine::GetInstance()->AddScriptObject(this, 0);
	return S_OK;
}

void CSideQuestHud::Update(_double deltaTime)
{
	std::thread updateUI(ThreadUpate, this,std::ref(mutexUpdate));
	updateUI.detach();
}

void CSideQuestHud::LateUpdate(_double deltaTime)
{
}

void CSideQuestHud::Render()
{
}

void CSideQuestHud::UpdateList()
{
	_uint activeQuest = 0;
	size_t i = 0;
	for (i = 0; i < m_QuestList.size(); i++) {
		CQuestData questData(*m_QuestList[i].second);
		if (i > 3)
			continue;
		list<CGameObject*> child = m_pQuestSubList[i]->GetChildren();
		auto& iter = child.begin();
		CText* questName = static_cast<CText*>((*iter)->GetComponent("Com_Text"));
		questName->SetString(questData.questName);
		iter++;
		CText* questPurpose = static_cast<CText*>((*iter)->GetComponent("Com_Text"));
		CText*	purpose = static_cast<CText*>((*iter)->GetComponent("Com_Text"));
		_int objCnt = questData.purposeCount;
		_int objNum = questData.purposeNumber;
		_int startCnt = questData.questStartCnt;
		string purposStr;
		if (objNum - objCnt > 0) {
			purposStr = questData.questPurpose +
				to_string(objCnt - startCnt) + " / " +
				to_string(objNum - startCnt);
		}
		else {
			purposStr = questData.questPurpose +
				to_string(objNum - startCnt) + " / " +
				to_string(objNum - startCnt);
		}
		
		purpose->SetString(purposStr);
		m_pQuestSubList[i]->SetActive(true);
	}
	for (; i < m_pQuestSubList.size(); i++) {
		m_pQuestSubList[i]->SetActive(false);
	}
}

void CSideQuestHud::LinkQuestList(vector<pair<CEmptyUI*, CQuestData*>>& _list)
{
	m_QuestList = _list;
}

CSideQuestHud * CSideQuestHud::Create(CGameObject * pTarget)
{
	CSideQuestHud*	obj = new CSideQuestHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CSideQuestHud::Free()
{
}

