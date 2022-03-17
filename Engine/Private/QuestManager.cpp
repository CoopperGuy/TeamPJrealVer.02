#include "EnginePCH.h"
#include "..\Public\QuestManager.h"
#include "Engine.h"

IMPLEMENT_SINGLETON(CQuestManager)

static string _path = "../../DB/QuestDB.xlsx";

CQuestManager::CQuestManager()
{
}

HRESULT CQuestManager::LoadQeustData()
{
	if (CEngine::USAGE::USAGE_TOOL == CEngine::GetInstance()->GetCurrentUsage()) {
		return S_OK;
	}
	xlnt::workbook wb;
	wb.load(_path.c_str());
	xlnt::worksheet ws = wb.active_sheet();
	_uint i = 0;
	for (auto& row : ws.rows(true)) {
		i++;
		if (i == 1)
			continue;
		_int questId = stoi(row[0].to_string());
		string npcIdStr = row[1].to_string();
		string QuestName = row[2].to_string();
		string questPurpose = row[3].to_string();
		string questObj = row[4].to_string();
		_int purposeNumber = stoi(row[5].to_string());

		CQuestData* questData = new CQuestData(npcIdStr, QuestName, questPurpose, questObj, purposeNumber);

		m_QuestList.emplace(make_pair(questId, questData));

		/*	for (int j = 3; j < row.length(); j++) {
				string purpos = row[j].to_string();
				questData->
			}*/
	}
	//questID must be n0000. n000 is questBox

	return S_OK;
}

void CQuestManager::AcceptQuest(_int _qusetID)
{
	auto& iter = m_QuestList.find(_qusetID);
	if (iter != m_QuestList.end()) {
		CDeadInfo info = CEngine::GetInstance()->GetDeadObjInfo((*iter).second->purposeObj);
		(*iter).second->SetStartQuest(info.GetDeadCount());
	}
}

CQuestData* CQuestManager::GetQuestData(_int _questID)
{
	auto iter = m_QuestList.find(_questID);
	if (iter != m_QuestList.end()) {
		return (*iter).second;
	}
	return nullptr;
}

void CQuestManager::Free()
{
	for (auto& iter : m_QuestList) {
		SafeRelease(iter.second);

	}
	m_QuestList.clear();
	SafeRelease(m_QuestObject);
}
