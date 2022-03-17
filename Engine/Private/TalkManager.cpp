#include "EnginePCH.h"
#include "..\Public\TalkManager.h"
#include "Engine.h"

IMPLEMENT_SINGLETON(CTalkManager)

static string talkPath = "../../DB/TalkDB.xlsx";

CTalkManager::CTalkManager()
{
}

HRESULT CTalkManager::LoadTalkData()
{
	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL) {
		return S_OK;
	}
	xlnt::workbook wb;
	wb.load(talkPath.c_str());
	xlnt::worksheet ws = wb.active_sheet();
	_uint i = 0;
	for (auto& row : ws.rows(true)) {
		i++;
		if (i == 1)
			continue;
		_int talkID = stoi(row[0].to_string());
		vector<string> datas;
		for (int j = 1; j < row.length(); j++) {
			string talk = row[j].to_string();
			for (auto& iter : talk) {
				if (iter == '\\')
					iter = '\n';
			}
			if(talk != "")
				datas.emplace_back(talk);
		}
		

		if (talkID >= 1000 && talkID < 10000) {
			talkBoxData.emplace(make_pair(talkID, datas));
		}
		else {
			talkDeskData.emplace(make_pair(talkID, datas));
		}
	}
	return S_OK;
}

vector<string> CTalkManager::GetTalk(_int talkID)
{
	if (talkID >= 1000 && talkID < 10000) {
		auto iter = talkBoxData.find(talkID);
		if (iter != talkBoxData.end()) {
			vector<string> texts = (*iter).second;
			return texts;
		}
	}
	else {
		auto iter = talkDeskData.find(talkID);
		if (iter != talkDeskData.end()) {
			vector<string> texts = (*iter).second;
			return texts;
		}
	}
	return vector<string>();
}

string CTalkManager::GetTalk(_int talkID, _int talkIdx)
{
	//n000 = box, n0000 = desk and This Mean QuestID
	//n00 = npcID
	//n ~ n0 = questProgress
	//ex 50301 // 50000 quest npc 300 talk 1 
	if (talkID >= 1000 && talkID < 10000) {
		auto iter = talkBoxData.find(talkID);
		if (iter != talkBoxData.end()) {
			vector<string> texts = (*iter).second;
			return texts[talkIdx];
		}
	}
	else {
		auto iter = talkDeskData.find(talkID);
		if (iter != talkDeskData.end()) {
			vector<string> texts = (*iter).second;
			return texts[talkIdx];
		}
	}
	return string();
}

size_t CTalkManager::GetTalkSize(_int talkID)
{
	if (talkID >= 1000 && talkID < 10000) {
		auto iter = talkBoxData.find(talkID);
		if (iter != talkBoxData.end()) {
			vector<string> texts = (*iter).second;
			return texts.size();
		}
	}
	else {
		auto iter = talkDeskData.find(talkID);
		if (iter != talkDeskData.end()) {
			vector<string> texts = (*iter).second;
			return texts.size();
		}
	}
	return 0;
}

void CTalkManager::Free()
{
	for (auto& iter : talkDeskData) {
		iter.second.clear();
	}
	talkDeskData.clear();
	for (auto& iter : talkBoxData) {
		iter.second.clear();
	}
	talkBoxData.clear();
}
