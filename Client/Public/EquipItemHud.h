#pragma once

#include "Engine.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "IScriptObject.h"
#include "Client_Enum.h"
#include "Inventory.h"
#include "ItemInfo.h"
BEGIN(Client)

class CEquipItemHud :
	public IScriptObject
{

private:
	explicit CEquipItemHud();
	virtual ~CEquipItemHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void AddEquipItem(CEmptyUI* item, EQUIPTYPE _category);
	void RemoveEquipItem(_uint idx, EQUIPTYPE _category);
	void CheckItems();
	void LinkInven(CInventory* inven);
	void Scroll(_double deltaTime);
public:
	void SetAddPosition(_float x);
	void AddThread() { m_iCurThread++; }
	void MinusThread() { m_iCurThread--; }
public:
	CItem*	GetSelectedItem();
public:
	_int GetThreadNum() { return m_iCurThread; }
private:
	vector<pair<CEmptyUI*, ITEMSUB>>	m_pEquipItemList[(_int)EQUIPTYPE::TYPE_END];
	vector<CEmptyUI*>	m_pEquipTagList;
	CGameObject*		m_pTarget = nullptr;
	CEmptyUI*			m_pThisUI = nullptr;
private:
	CInventory*			m_pInven = nullptr;
	CItemInfo*			m_pItemInfo = nullptr;
private:
	_uint				m_iCurSelectedTag = 0;
	_uint				m_iCurSelected = 0;
	_float				m_fCurYSize[(_int)EQUIPTYPE::TYPE_END];
	const _float		m_constYScale = 65.f;
	const _float		m_fCorrectY = 120.f;
	_float				m_fAddItemPosX = 390.f;
	
	_int				m_iCurThread = 0;
private:
	_bool				m_bisActive = false;
public:
	static CEquipItemHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END