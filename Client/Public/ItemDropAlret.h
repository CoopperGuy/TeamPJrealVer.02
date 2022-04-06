#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CItemDropAlret :
	public IScriptObject
{
private:
	explicit CItemDropAlret();
	virtual ~CItemDropAlret() = default;
public:
	virtual HRESULT Initailze(string _itemName, CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CEmptyUI*	m_pThisUI = nullptr;
	CText*		m_pText = nullptr;
	CRectTransform*	m_pTransform = nullptr;
private:
	_double		m_LifeDelta = 0;
	_double		m_LifeTime = 0.5;
public:
	static CItemDropAlret* Create(string _itemName, CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END