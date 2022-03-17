#pragma once

#include "Engine.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "IScriptObject.h"
#include "Inventory.h"
BEGIN(Client)

class CBackPackHud;
class CQuestHud;

class CMenuHud :
	public IScriptObject
{
	enum MENULIST
	{
		MENU_EQUIP,
		MENU_QUEST,
		MENU_END
	};
private:
	explicit CMenuHud();
	virtual ~CMenuHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void	MenuEmplace(CEmptyUI* ui) { m_pMenuButton.emplace_back(ui); }
	void	AddInvenInBackPackHud(CInventory* inven);
	void	LinkInventoryToHud(CInventory* inven);
public:
	void SetInvenRightLeft(_bool right) { m_bRightOpenInven = right; }
public:
	_bool GetInvenRightLeft() { return m_bRightOpenInven; }
private:
	vector<CEmptyUI*>	m_pMenuButton;
	CEmptyUI*			m_pMenuList[MENU_END] = { nullptr };
	CGameObject*		m_pTarget = nullptr;
	CEmptyUI*			m_pThisUI = nullptr;
private:
	CBackPackHud*		m_pBackPack = nullptr;
	CQuestHud*			m_pQuestHud = nullptr;
	CInventory*			m_pInven = nullptr;
private:
	_bool				m_bisActive = false;
	_bool				m_bRightOpenInven = false;
public:
	static CMenuHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END