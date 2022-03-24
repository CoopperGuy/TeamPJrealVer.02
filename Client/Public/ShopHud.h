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
	void SetShopLength(_float _length);
	void SetOffset(_float _length);
private:
	CEmptyUI*		m_pThisUI = nullptr;
	CEmptyUI*		m_pBuy = nullptr;
	CEmptyUI*		m_pScrollBar = nullptr;
	CEmptyUI*		m_pScrollBG = nullptr;
	CEmptyUI*		m_pShopList = nullptr;
 private:
	_float			m_fShopLength = 0.f;
	_float			m_fcorrectionYSize = 0.f;
private:
	_bool			m_bIsScrolled = false;
public:
	static CShopHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END