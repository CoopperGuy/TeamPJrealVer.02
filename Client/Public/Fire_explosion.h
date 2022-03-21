#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CFire_explosion : public CBasicEffect
{
private:
	explicit CFire_explosion();
	virtual ~CFire_explosion() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTarget);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;
private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CEmptyEffect*   m_pBoob		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	CTransform*		m_pTargetTrans = nullptr;
	CTransform*		m_pChildren	   = nullptr;

private:
	_double			m_Duration = 2.0;
	_double			m_DurationDelta = 0;
	_float			m_fScale = 0.f;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* _pTarget);
	virtual void Free();
};

END