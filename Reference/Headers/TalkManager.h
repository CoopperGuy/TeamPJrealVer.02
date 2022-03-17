#pragma once
#include "Base.h"

BEGIN(Engine)

class CTalkManager :
	public CBase
{
	DECLARE_SINGLETON(CTalkManager)
	typedef unordered_map<_int, vector<string>> TALKDATA;
public:
	CTalkManager();
	virtual ~CTalkManager() = default;
public:
	HRESULT	LoadTalkData();
public:
	vector<string> GetTalk(_int talkID);
	string GetTalk(_int talkID, _int talkIdx);
	size_t	GetTalkSize(_int talkID);
private:
	TALKDATA talkDeskData; // talk(quest)id , talkData
	TALKDATA talkBoxData;
public:
	virtual void Free() override;
};

END