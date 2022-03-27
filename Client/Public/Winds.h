#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CWinds :
	public CBasicEffect
{
private:
	explicit CWinds();
	virtual ~CWinds() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* _pThisEffect, CGameObject* _pPlayer);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
private:
	CEmptyEffect*	m_pThisEffect = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
private:
	CTransform*		m_pPlayerTrans = nullptr;
private:
	_double			m_Duration = 0.5;
	_double			m_DurationDelta = 0;
public:
	static CWinds* Create(CEmptyEffect* _pThisEffect, CGameObject* _pPlayerEffect);
	virtual void Free();
};

END