#pragma once
#ifndef __QUESTDATA_H__
#define  __QUESTDATA_H__
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CQuestData final :
	public CBase
{
public:
	CQuestData();
	CQuestData(const CQuestData& rhs);
	CQuestData(string ids, string Name,string purpos, string obj , _int num);
	virtual ~CQuestData() = default;
public:
	_bool isQuestAcheive();
	_bool isQuestClear();
public:
	void SetStartQuest(_int startCount);
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
public:
	virtual void Free() override;

};

END


#endif