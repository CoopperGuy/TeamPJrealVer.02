#pragma once
#include "IScriptObject.h"

BEGIN(Client)

class CMouseUI :
	public IScriptObject
{
private:
	explicit CMouseUI();
	virtual ~CMouseUI() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CEmptyUI*	m_pThisUI = nullptr;
public:
	static CMouseUI* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END