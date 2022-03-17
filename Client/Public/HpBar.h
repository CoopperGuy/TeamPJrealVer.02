#pragma once

#include "Engine.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "IScriptObject.h"

BEGIN(Client)

class CHpBar final :
	public IScriptObject
{
private:
	explicit CHpBar();
	virtual ~CHpBar() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CGameObject*		pTarget = nullptr;
	CStat*				m_pTargetStat = nullptr;

	CEmptyUI*			m_pHpBar = nullptr;
	CEmptyUI*			m_pSTABar = nullptr;
	CEmptyUI*			m_pExpBar = nullptr;
public:				
	_float	m_fPercetage = 1.f;
	_float	m_fBackPercentage = 1.f;
	_float	m_fBackPercentageSta = 1.f;
	_float	m_fBackPercentageExp = 1.f;

public:
	static CHpBar* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END