#include "EnginePCH.h"
#include "..\Public\AlretManager.h"
#include "QuestData.h"
IMPLEMENT_SINGLETON(CAlretManager)

CAlretManager::CAlretManager()
{
}

void CAlretManager::AddDeadObject(string objName)
{
	auto& iter = m_DeadObject.find(objName);
	if (iter != m_DeadObject.end()) {
		(*iter).second.AddDeadCount();
	}
	else {
		m_DeadObject.emplace(make_pair(objName, CDeadInfo()));
	}
}

void CAlretManager::AddAcceptQuest(_int questID, CQuestData* questData)
{
	m_AccepQuest.emplace(make_pair(questID, questData));
}

void CAlretManager::AddClearQuest(_int questID, CQuestData* questData)
{
	m_ClearQuest.emplace(make_pair(questID, questData));
}

void CAlretManager::QuestClear(_int questID)
{
	auto iter = m_AccepQuest.find(questID);
	if (iter != m_AccepQuest.end()) {
		(*iter).second->isClear = true;
		m_ClearQuest.emplace(make_pair(questID, (*iter).second));
		m_AccepQuest.erase(iter);
	}
}

void CAlretManager::RemoveAcceptQuest(_int questID)
{
	auto iter = m_AccepQuest.find(questID);
	if (iter != m_AccepQuest.end()) {
		iter = m_AccepQuest.erase(iter);
	}
}

_bool CAlretManager::SearchAcceptQuest(_int questID)
{
	auto& iter = m_AccepQuest.find(questID);
	if (iter == m_AccepQuest.end()) {
		return false;
	}
	return true;
}

_bool CAlretManager::SearchClearQuest(_int questID)
{
	auto& iter = m_ClearQuest.find(questID);
	if (iter == m_ClearQuest.end()) {
		return false;
	}
	return true;
}

CDeadInfo CAlretManager::GetDeadObjInfo(string objName)
{
	auto& iter = m_DeadObject.find(objName);
	if (iter == m_DeadObject.end()) {
		m_DeadObject.emplace(make_pair(objName,CDeadInfo()));
		return CDeadInfo();
	}
	return (*iter).second;
}

void CAlretManager::SetQuestList(unordered_map<_int, CQuestData*>&& quest)
{
	m_AccepQuest = std::move(quest);
}

unordered_map<_int, CQuestData*> CAlretManager::GetQuestList()
{
	return std::move(m_AccepQuest);
}

void CAlretManager::Free()
{
	//leak waring
	m_AccepQuest.clear();
	m_ClearQuest.clear();
	m_DeadObject.clear();
}
