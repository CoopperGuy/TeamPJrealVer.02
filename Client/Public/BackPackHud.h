#pragma once

#include "Engine.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "IScriptObject.h"
#include "Client_Enum.h"
#include "Inventory.h"
#include "Item.h"

BEGIN(Client)

class CEquipItemHud;
class CMaterialHud;
class CConsumItemHud;
class CReinforceHud;
class CBackPackHud :
	public IScriptObject
{

private:
	explicit CBackPackHud();
	virtual ~CBackPackHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void	MenuEmplace(CEmptyUI* ui) { m_pBackPackList.emplace_back(ui); }
	void	LinkInventoryToHud(CInventory* inven) { m_pInven = inven; }
public:
	void	SetRightLeft(_bool right) { m_bisOpenRightLeft = right; }
public:
	void	RemoveItem(ITEMTYPE _type,_int _idx);
private:
	vector<CEmptyUI*>	m_pBackPackList;
	vector<CEmptyUI*>	m_pBackPackButton;
	CGameObject*		m_pTarget = nullptr;
	CEmptyUI*			m_pThisUI = nullptr;
	CEmptyUI*			m_pReinforceButton = nullptr;
private:
	CEquipItemHud*		m_pEquipHud = nullptr;
	CMaterialHud*		m_pMaterialHud = nullptr;
	CConsumItemHud*		m_pConsumHud = nullptr;
	CReinforceHud*		m_pReinforceHud = nullptr;
private:
	CInventory*			m_pInven = nullptr;
private:
	_uint				m_iCurSelected = 0;
private:
	_bool				m_bisActive = false;
	_bool				m_bisOpenRightLeft = false;
	_bool				m_bIsReinforceMenu = false;
private:
	mutable _float		m_fAddPosX = 390.f;
public:
	static CBackPackHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END