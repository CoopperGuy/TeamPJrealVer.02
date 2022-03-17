#pragma once

#include "Engine.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "IScriptObject.h"
#include "Inventory.h"
BEGIN(Client)

class CMaterialHud :
	public IScriptObject
{
private:
	explicit CMaterialHud();
	virtual ~CMaterialHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void AddMaterialItem(CEmptyUI* item);
	void RemoveMaterialItem(_uint idx);
public:
	void LinkInven(CInventory* inven);
	void CheckItems();
	void Scroll(_double deltaTime);
	void AddThread() { m_iCurThread++; }
	void MinusThread() { m_iCurThread--; }
public:
	_int GetThreadNum() { return m_iCurThread; }
public:
	void SetAddPosition(_float x);
private:
	vector<pair<CEmptyUI*, ITEMSUB>>	m_pMaterialList;
	CGameObject*		m_pTarget = nullptr;
	CEmptyUI*			m_pThisUI = nullptr;
private:
	CInventory*			m_pInven = nullptr;
private:
	_uint				m_iCurSelectedTag = 0;
	_uint				m_iCurSelected = 0;
	_float				m_fCurYSize = 0.f;
	const _float		m_constYScale = 65.f;
	_float				m_fAddItemPosX = 390.f;

	_int				m_iCurThread = 0;
private:
	_bool				m_bisActive = false;
public:
	static CMaterialHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END