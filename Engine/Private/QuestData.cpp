#include "EnginePCH.h"
#include "..\Public\QuestData.h"

USING(Engine)

CQuestData::CQuestData()
{
}

CQuestData::CQuestData(const CQuestData & rhs)
	:questName(rhs.questName),
	questPurpose(rhs.questPurpose),
	purposeObj(rhs.purposeObj),
	purposeNumber(rhs.purposeNumber),
	purposeCount(rhs.purposeCount),
	questStartCnt(rhs.questStartCnt),
	questProgress(rhs.questProgress)
{
	for (auto& iter : rhs.npcID) {
		this->npcID.emplace_back(iter);
	}
	/*
	public:
	vector<_int>	npcID;
	string			questName;
	string			questPurpose;
	string			purposeObj;
	_int			purposeNumber;
	_int			purposeCount = 0;
	_int			questStartCnt = 0;
	_int			questProgress = 0;
public:
	_bool			isAchieve = false;
	_bool			isClear = false;
	*/
}

CQuestData::CQuestData(string ids, string Name, string purpos, string obj, _int num)
{
	string id;
	for (auto&iter : ids) {
		if (iter == '/') {
			npcID.emplace_back(stoi(id));
			id.clear();
			continue;
		}
		id += iter;
	}

	questName = Name;
	questPurpose = purpos;
	purposeObj = obj;
	purposeNumber = num;
}

_bool CQuestData::isQuestAcheive()
{
	if (purposeNumber <= purposeCount) {
		isAchieve = true;
	}
	else {
		isAchieve = false;
	}
	return isAchieve;
}

_bool CQuestData::isQuestClear()
{
	return isClear;
}

void CQuestData::SetStartQuest(_int startCount)
{
	purposeNumber += startCount;
	purposeCount = startCount;
	questStartCnt = startCount;
}

void CQuestData::Free()
{
}
