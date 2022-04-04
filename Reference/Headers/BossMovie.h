#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Engine)

class CBossMovie :
	public IScriptObject
{
private:
	explicit CBossMovie();
	virtual ~CBossMovie() = default;
public:
	virtual HRESULT Initailze(_int _type, CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CEmptyUI*	m_pThisUI = nullptr;
	CVIBuffer_RectUI*	m_pName = nullptr;
	CVIBuffer_RectUI*	m_pSubName = nullptr;
private:
	_double		m_LifeTime = 2;
	_double		m_LifeDelta = 0;
public:
	static CBossMovie* Create(_int _type = 0, CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END