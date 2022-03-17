#include "stdafx.h"
#include "..\Public\QuestHud.h"

BEGIN(Client)
void UpdateQuestList(CQuestHud* thisUI) {
	thisUI->AddQuestAtList();
}
END

USING(Client)

CQuestHud::CQuestHud()
{
}

HRESULT CQuestHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
	}
	m_pThisUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "QuestHud"));
	m_pQuestPos = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "QuestList"));
	_float2 pos = m_pQuestPos->GetPosition();
	m_fQuestYPos = pos.y + 180.f;
	m_pSideQuestHud = CSideQuestHud::Create(m_pTarget);
	return S_OK;
}

void CQuestHud::Update(_double deltaTime)
{
	CEngine::GetInstance()->GetQuestData(10);
	
	m_pSideQuestHud->LinkQuestList(m_QuestList);
	std::thread updateQuest(UpdateQuestList, this);
	updateQuest.detach();

	this->CheckQuestProgress();
	this->RemoveQuest(0);
}

void CQuestHud::LateUpdate(_double deltaTime)
{
}

void CQuestHud::Render()
{
}

void CQuestHud::AddQuestAtList()
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	unordered_map<_int, CQuestData*> acceptQuest = CEngine::GetInstance()->GetQuestList();
	size_t acceptQuestCnt = acceptQuest.size();
	size_t curQuestList = m_QuestList.size();
	
	for (int i = 0; i < acceptQuestCnt - curQuestList; i++) {
		CEmptyUI*	questObj = static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_Quest"));
		questObj->SetActive(false);
		_float2 pos = questObj->GetPosition();
		questObj->SetPosition(pos.x, m_fQuestYPos);
		m_fQuestYPos += m_fYPosPadding;
		m_QuestList.emplace_back(questObj, nullptr);
	}
	_uint i = 0;
	for (auto& iter : acceptQuest) {
		m_QuestList[i].second = iter.second;

		list<CGameObject*> child = m_QuestList[i].first->GetChildren();
		auto& iter = child.begin();
		CText* text = static_cast<CText*>((*iter)->GetComponent("Com_Text"));
		text->SetString(m_QuestList[i].second->questName);
		m_pQuestPos->AddChild(m_QuestList[i].first);

		i++;
	}
	CEngine::GetInstance()->SetQuestList(std::move(acceptQuest));
}

void CQuestHud::CheckQuestProgress()
{
	for (auto& iter : m_QuestList) {
		CDeadInfo deadObjInfo = CEngine::GetInstance()->GetDeadObjInfo(iter.second->purposeObj);
		_int purposeCnt = iter.second->purposeCount;
		_int purposeNum = iter.second->purposeNumber;
		_int deadObjInfoCnt = deadObjInfo.GetDeadCount();
		if (purposeCnt != deadObjInfoCnt) {
			iter.second->purposeCount += (deadObjInfoCnt - purposeCnt);
		}
		iter.second->isQuestAcheive();
	}
}

void CQuestHud::RemoveQuest(_int questID)
{
	auto&iter = m_QuestList.begin();
	for (; iter != m_QuestList.end();) {
		if ((*iter).second->isClear) {
			iter = m_QuestList.erase(iter);
		}
		else {
			iter++;
		}
	}
	_float y = 0;
	for (auto& iter : m_QuestList) {
		_float2 pos = iter.first->GetPosition();
		iter.first->SetPosition(pos.x,y);
		y += m_fYPosPadding;
	}
	m_fQuestYPos = m_fYPosPadding;
}

CQuestHud * CQuestHud::Create(CGameObject * pTarget)
{
	CQuestHud*	obj = new CQuestHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CQuestHud::Free()
{
}

