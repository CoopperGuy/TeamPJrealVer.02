#pragma once

#include "Base.h"


BEGIN(Engine)

class CQuestData;

class CDeadInfo {
public:
	CDeadInfo() {

	}
	~CDeadInfo() {

	}
public:
	void AddDeadCount() { m_DeadCount++; }
public:
	_int GetDeadCount() { return m_DeadCount; }
private:
	_int m_DeadCount = 0;
};

class CAlretManager final :
	public CBase
{
	DECLARE_SINGLETON(CAlretManager)
private:
	CAlretManager();
	virtual ~CAlretManager() = default;
public:
	void AddDeadObject(string objName);
	void AddAcceptQuest(_int questID, CQuestData* questData);
	void AddClearQuest(_int questID, CQuestData* questData);
	void QuestClear(_int questID);
public:
	void RemoveAcceptQuest(_int questID);
public:
	_bool SearchAcceptQuest(_int questID);
	_bool SearchClearQuest(_int qeustID);
	CDeadInfo GetDeadObjInfo(string objName);
public:
	void	SetQuestList(unordered_map<_int, CQuestData*>&& quest);
	unordered_map<_int, CQuestData*>	GetQuestList();

private:
	unordered_map<_int, CQuestData*>	m_AccepQuest;
	unordered_map<_int, CQuestData*>	m_ClearQuest;
	unordered_map<string, CDeadInfo>	m_DeadObject;
	//m_AcceptQuest;
	//m_DeadObject;
	//m_ClearQuest;
public:
	virtual void Free() override;
};



END