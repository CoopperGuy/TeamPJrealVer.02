#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CReinforceFall :
	public IScriptObject
{
private:
	explicit CReinforceFall();
	virtual ~CReinforceFall() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CEmptyUI*	m_pThisUI = nullptr;
private:
	_double	m_LifeDelta = 0;
	_double m_LifeTime = 0.8;
public:
	static CReinforceFall* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END