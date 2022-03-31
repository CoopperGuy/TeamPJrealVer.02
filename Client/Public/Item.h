#pragma once

#include "Engine.h"
#include "Client_Struct.h"
#include "Client_Enum.h"
BEGIN(Client)

class CItem 
{
public:
	CItem();
	CItem(ITEMINFO _info);
	CItem(CItem* rhs);
	~CItem();
public:
	ITEMTYPE	GetItemType() { return m_tinfo.itemType; }
	EQUIPTYPE	GetEquipType() { return m_tinfo.equipType; }
	ITEMINFO	GetItempInfo() { return m_tinfo; }
	void		IncreaseItemAmount(_uint amount = 1) { m_tinfo.numOfItem += amount; }
	void		UseItem() { m_tinfo.numOfItem--; }
	void		ItemLevelUp() { m_tinfo.level++; }
private:
	ITEMINFO m_tinfo;
};

END