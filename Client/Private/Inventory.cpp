#include "stdafx.h"
#include "..\Public\Inventory.h"

USING(Client)

CInventory::CInventory()
{

}

HRESULT CInventory::Initailze(CGameObject * pArg)
{
	return S_OK;
}

void CInventory::Update(_double deltaTime)
{
	for (int i = 0; i < (_int)ITEMTYPE::TYPE_END; i++) {
		auto iter = m_Inven[i].begin();
		for (; iter != m_Inven[i].end();) {
			if ((*iter).second->GetItempInfo().numOfItem == 0) {
				iter = m_Inven[i].erase(iter);
			}
			else {
				iter++;
			}
		}
	}
}

void CInventory::LateUpdate(_double deltaTime)
{
}

void CInventory::Render()
{
}

_bool CInventory::AddItem(string name, CItem * item)
{
	_int itemType = (_int)item->GetItemType();
	_bool isCreated = false;
	if (itemType == (_int)ITEMTYPE::EQUIP) {
		_int equipType = (_int)item->GetEquipType();
		auto& iter = m_Eqiup[equipType].find(name);
		if (iter != m_Eqiup[equipType].end()) {
			CItem* itemInfo = iter->second;
			itemInfo->IncreaseItemAmount();
			isCreated = false;
		}
		else {
			m_Eqiup[equipType].emplace(name, item);
			isCreated = true;
		}
	}
	else {
		auto& iter = m_Inven[itemType].find(name);
		if (iter != m_Inven[itemType].end()) {
			CItem* temp = iter->second;
			temp->IncreaseItemAmount();
			isCreated = false;
		}
		else {
			m_Inven[itemType].emplace(name, item);
			isCreated = true;
		}
	}

	return isCreated;
}

_bool CInventory::UseItem(string name, ITEMTYPE type)
{
	_int itemType = (_int)type;
	if (itemType == (_int)ITEMTYPE::EQUIP) {
		return false;
	}
	else {
		auto& iter = m_Inven[itemType].find(name);
		if (iter != m_Inven[itemType].end()) {
			iter->second->UseItem();
			return true;
		}
	}
	return false;
}

void CInventory::RemoveItem(string name, ITEMTYPE type, EQUIPTYPE equipType)
{
	_int itemType = (_int)type;
	if (itemType == (_int)ITEMTYPE::EQUIP) {
		_int eType = (_int)equipType;
		auto& iter = m_Eqiup[eType].find(name);
		if (iter != m_Eqiup[eType].end()) {
			delete iter->second;
			iter->second = nullptr;
			iter = m_Eqiup[eType].erase(iter);
		}
	}
	else {
		auto& iter = m_Inven[itemType].find(name);
		if (iter != m_Inven[itemType].end()) {
			delete iter->second;
			iter->second = nullptr;
			iter = m_Inven[itemType].erase(iter);
		}
	}
}

void CInventory::ReduceItem(string name, ITEMTYPE type, EQUIPTYPE equipType)
{
	_int itemType = (_int)type;
	if (itemType == (_int)ITEMTYPE::EQUIP) {
		_int eType = (_int)equipType;
		auto& iter = m_Eqiup[eType].find(name);
		if (iter != m_Eqiup[eType].end()) {
			delete iter->second;
			iter->second = nullptr;
			iter = m_Eqiup[eType].erase(iter);
		}
	}
	else {
		auto& iter = m_Inven[itemType].find(name);
		if (iter != m_Inven[itemType].end()) {
			delete iter->second;
			iter->second = nullptr;
			iter = m_Inven[itemType].erase(iter);
		}
	}
}

void CInventory::SellItem(string name, ITEMTYPE type, EQUIPTYPE equipType)
{
	_int itemType = (_int)type;
	if (itemType == (_int)ITEMTYPE::EQUIP) {
		_int eType = (_int)equipType;
		auto& iter = m_Eqiup[eType].find(name);
		if (iter != m_Eqiup[eType].end()) {
			delete iter->second;
			iter->second = nullptr;
			iter = m_Eqiup[eType].erase(iter);
		}
	}
	else {
		auto& iter = m_Inven[itemType].find(name);
		if (iter != m_Inven[itemType].end()) {
			delete iter->second;
			iter->second = nullptr;
			iter = m_Inven[itemType].erase(iter);
		}
	}
}

INVEN CInventory::GetInventoryByType(ITEMTYPE _type, EQUIPTYPE equipType)
{
	if (_type == ITEMTYPE::EQUIP) {
		return m_Eqiup[(_int)equipType];
	}
	return m_Inven[(_int)_type];
}

_uint CInventory::GetInvenTypeSize(ITEMTYPE _type, EQUIPTYPE equipType)
{
	if (_type == ITEMTYPE::EQUIP) {
		return (_uint)m_Eqiup[(_int)equipType].size();
	}
	return (_uint)m_Inven[(_int)_type].size();
}

CItem * CInventory::GetItemByName(string name, ITEMTYPE type, EQUIPTYPE equipType)
{
	if (type == ITEMTYPE::EQUIP) {
		auto iter = m_Eqiup[(_int)equipType].find(name);
		if (iter != m_Eqiup[(_int)equipType].end()) {
			return (*iter).second;
		}
	}
	auto iter = m_Inven[(_int)type].find(name);
	if (iter != m_Inven[(_int)type].end()) {
		return (*iter).second;
	}
	return nullptr;
}

CInventory * CInventory::Create(CGameObject * pTarget)
{
	CInventory*	obj = new CInventory();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CInventory::Free()
{
	for (_uint i = 0; i < (_int)ITEMTYPE::TYPE_END; i++) {
		for (auto& iter : m_Inven[i]) {
			delete iter.second;
			iter.second = nullptr;
		}
		m_Inven[i].clear();
	}
}

