#include "EnginePCH.h"
#include "..\Public\XlsxRead.h"
#include "Engine.h"

IMPLEMENT_SINGLETON(CXlsxRead)

CXlsxRead::CXlsxRead()
{
}

HRESULT CXlsxRead::Ready_XlsxLoader(string _path)
{
	if (CEngine::USAGE::USAGE_TOOL == CEngine::GetInstance()->GetCurrentUsage()) {
		return S_OK;
	}
	xlnt::workbook wb;
	wb.load(_path.c_str());
	xlnt::worksheet ws = wb.active_sheet();
	_uint i = 0;
	for (auto& row : ws.rows(true)) {
		if (i == 0) {
			i++;
			continue;
		}
		string tag = row[0].to_string();

		ITEMINFO	item;
		item.itemType = (ITEMTYPE)stoi(row[1].to_string().c_str());
		item.equipType = (EQUIPTYPE)stoi(row[2].to_string().c_str());
		item.numOfItem = stoi(row[3].to_string().c_str());
		item.hp = stof(row[4].to_string().c_str());
		item.atk = stof(row[5].to_string().c_str());
		item.def = stof(row[6].to_string().c_str());
		item.price = stoi(row[7].to_string().c_str());
		item.imageName = row[8].to_string();
		item.itemName = tag;
		m_itemDB.emplace(tag, item);

	}
	m_bIsFinish = true;

	return S_OK;
}

pair<string, ITEMINFO> CXlsxRead::GetItemAsName(string name)
{
	auto& iter = m_itemDB.find(name);
	if (iter != m_itemDB.end()) {
		return *iter;
	}
	return pair<string, ITEMINFO>{};
}

list<pair<string, ITEMINFO>> CXlsxRead::GetItemTypeList(ITEMTYPE _type)
{
	list<pair<string, ITEMINFO>> returnItem;
	for (auto& iter : m_itemDB) {
		if (iter.second.itemType == _type) {
			returnItem.emplace_back(iter);
		}
	}
	return returnItem;
}


void CXlsxRead::Free()
{
	m_itemDB.clear();
}
