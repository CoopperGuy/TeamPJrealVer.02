#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CChargeAxe :
	public CBasicEffect
{
private:
	explicit CChargeAxe();
	virtual ~CChargeAxe() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* _pThisEffect, CGameObject* _pPlayer);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
private:
	CEmptyEffect*	m_pThisEffect = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
private:
	CTransform*		m_pAxeTrans = nullptr;
private:
	_double			m_Duration = 0.5;
	_double			m_DurationDelta = 0;
private:
	_float			m_fScale = 0.3f;
public:
	static CChargeAxe* Create(CEmptyEffect* _pThisEffect, CGameObject* _pPlayerEffect);
	virtual void Free();

};

END