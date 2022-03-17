#pragma once

#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CShopHud :
	public IScriptObject
{
private:
	explicit CShopHud();
	virtual ~CShopHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	_bool IsBuySelected();
public:
	void SetActive(_bool isActive);
private:
	CEmptyUI*		m_pThisUI = nullptr;
	CEmptyUI*		m_pBuy = nullptr;
public:
	static CShopHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END