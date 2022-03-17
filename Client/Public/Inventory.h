#pragma once

#include "Engine.h"
#include "IScriptObject.h"
#include "Item.h"
#include "Client_Enum.h"

BEGIN(Client)

typedef unordered_map<string, CItem*> INVEN;

class CInventory :
	public IScriptObject
{
private:
	explicit CInventory();
	virtual ~CInventory() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	_bool	AddItem(string name, CItem* item);
	_bool	UseItem(string name, ITEMTYPE type);
	void	RemoveItem(string name, ITEMTYPE type, EQUIPTYPE equipType = EQUIPTYPE::TYPE_END);
	void	ReduceItem(string name, ITEMTYPE type, EQUIPTYPE equipType = EQUIPTYPE::TYPE_END);
	void	SellItem(string name, ITEMTYPE type, EQUIPTYPE equipType = EQUIPTYPE::TYPE_END);
public:
	INVEN	GetInventoryByType(ITEMTYPE _type, EQUIPTYPE equipType = EQUIPTYPE::TYPE_END);
	_uint	GetInvenTypeSize(ITEMTYPE _type, EQUIPTYPE equipType = EQUIPTYPE::TYPE_END);
	CItem*	GetItemByName(string name, ITEMTYPE type, EQUIPTYPE equipType = EQUIPTYPE::TYPE_END);
private:
	INVEN	m_Inven[(_int)ITEMTYPE::TYPE_END];
	INVEN	m_Eqiup[(_int)EQUIPTYPE::TYPE_END];

public:
	static CInventory* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END