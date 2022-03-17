#pragma once
#include "Base.h"
#include "QuestData.h"
#include "GameObject.h"
BEGIN(Engine)

class CQuestManager :
	public CBase
{
	DECLARE_SINGLETON(CQuestManager)
public:
	CQuestManager();
	virtual ~CQuestManager() = default;
public:
	HRESULT	LoadQeustData();
	void AcceptQuest(_int _qusetID);
public:
	CQuestData*	GetQuestData(_int _questID);
private:
	unordered_map<_int, CQuestData*> m_QuestList; // questid , questdata
	CGameObject*					m_QuestObject = nullptr;
public:
	virtual void Free() override;
};

END