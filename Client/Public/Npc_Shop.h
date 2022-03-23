#pragma once

#include "NPC.h"

BEGIN(Client)
class CShopHud;
class CPlayer;
class CNpc_Shop :
	public CNPC
{
private:
	explicit CNpc_Shop();
	explicit CNpc_Shop(CGameObject* pObj);
	virtual ~CNpc_Shop() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
public:
	void SetupShop();
public:
	ITEMINFO GetBuyingItem();
private:
	vector< pair<CEmptyUI*, ITEMINFO>>	m_ShopList;
	CEmptyGameObject*					m_pShopNPC = nullptr;
	CTransform*							m_pNpcTransform = nullptr;
	CShopHud*							m_pShopHud = nullptr;
	CEmptyUI*							m_pShopList = nullptr;
private:
	_int								m_iCurSelectedItem = -1;
	mutable _float						m_fAlretMakeDistance = 0.5f;
	_float								m_fShopListYLength = 0.f;
private:
	_bool								m_bisCreated = false;
	_bool								m_isActived = false;
public:
	static CNpc_Shop* Create(CGameObject* pTartget = nullptr);
	virtual void Free() override;
};

END