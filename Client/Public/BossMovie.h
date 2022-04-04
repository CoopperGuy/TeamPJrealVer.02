#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CBossMovie :
	public IScriptObject
{
private:
	explicit CBossMovie();
	virtual ~CBossMovie() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CEmptyUI*	m_pThisUI = nullptr;
private:
	_double		m_LifeTime = 1;
	_double		m_LifeDelta = 0;
public:
	static CBossMovie* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END