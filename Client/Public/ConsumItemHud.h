#pragma once
#include "Engine.h"
#include "IScriptObject.h"
#include "Inventory.h"
#include "Text.h"
#include "Texture.h"
BEGIN(Client)

class CConsumItemHud :
	public IScriptObject
{
	enum ITEMINFOSUB {SUB_NAME,SUB_AMOUNT,SUB_IMAGE,SUB_END};
private:
	explicit CConsumItemHud();
	virtual ~CConsumItemHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void AddConsumItem(CEmptyUI* item);
	void RemoveConsumItem(_uint idx);
	void AddQuickSlot();
public:
	void CheckItems();
	void LinkInven(CInventory* _inven);
	void Scroll(_double deltaTime);
public:
	void SetAddPosition(_float x);
public:
	void SetAddQuickSlotOpen(_bool tf) { m_bSelectInputIndex = tf; }
public:
	_bool GetAddQuickSlotOpen() { return m_bSelectInputIndex; }
	_int GetSelectedItemIndex() { return m_iCurSelected; }
	string GetSelectedItemName();
	CItem*	GetSelectedItem();
private:
	vector<pair<CEmptyUI*, ITEMSUB>>	m_pConsumItemList;
	CGameObject*						m_pTarget = nullptr;
	CEmptyUI*							m_pThisUI = nullptr;
private:
	CInventory*							m_pInven = nullptr;
private:
	_int								m_iCurSelected = 0;
	_float								m_fCurYSize = 0.f;
	const _float						m_constYScale = 65.f;
	_float								m_fAddItemPosX = 390.f;

private:
	_bool								m_bisActive = false;
	_bool						        m_bSelectInputIndex = false;
public:
	static CConsumItemHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END