#pragma once
#include "Component.h"

BEGIN(Engine)

class CEmptyUI;

class ENGINE_DLL CShop final :
	public CComponent
{
private:
	explicit CShop(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CShop(const CShop& rhs);
	virtual ~CShop() = default;

public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Initialize(void* pArg) override;
public:
	void SetupItemList(ITEMINFO _item);
public:
	ITEMINFO BuyItem();
	ITEMINFO SellItem();
public:
	void SetItemType(ITEMTYPE _type) { m_ItemType = _type; }
	void SetItemRange(_int _start, _int _end) { m_ItemRange = make_pair(_start, _end); }
public:
	ITEMTYPE GetItemType() { return m_ItemType; }
	pair<_int, _int>& GetItemRange() { return m_ItemRange; }
private:
	vector<pair<CEmptyUI*, ITEMINFO>>	m_vecItemList;
	ITEMTYPE							m_ItemType;
	pair<_int, _int>					m_ItemRange;
public:
	static CShop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END