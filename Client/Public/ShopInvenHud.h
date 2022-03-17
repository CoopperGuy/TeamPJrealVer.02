#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CShopInvenHud :
	public IScriptObject
{
private:
	explicit CShopInvenHud();
	virtual ~CShopInvenHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
//public:
//	void	MenuEmplace(CEmptyUI* ui) { m_pMenuButton.emplace_back(ui); }
//	void	AddInvenInBackPackHud(CInventory* inven);
//	void	LinkInventoryToHud(CInventory* inven);
//private:
//	vector<CEmptyUI*>	m_pMenuButton;
//	CEmptyUI*			m_pMenuList[MENU_END] = { nullptr };
//	CGameObject*		m_pTarget = nullptr;
//	CEmptyUI*			m_pThisUI = nullptr;
//private:
//	CInventory*			m_pInven = nullptr;
//private:
//	_bool				m_bisActive = false;
public:
	static CShopInvenHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END