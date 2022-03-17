#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CAddQuickslot :
	public IScriptObject
{
private:
	explicit CAddQuickslot();
	virtual ~CAddQuickslot() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void UpdateIcon(string path, _uint idx);
public:
	void SetIsOpen(_bool tf) { m_bisOpened = tf; }
public:
	_bool GetIsOpen() { return m_bisOpened; }
private:
	CEmptyUI*	m_pThisUI = nullptr;
	vector<CVIBuffer_RectUI*>	m_vecBuffer;
	CEmptyUI*	m_pCloseButton = nullptr;
private:
	_bool	m_bisOpened = false;

public:
	static CAddQuickslot* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END