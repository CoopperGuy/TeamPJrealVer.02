#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CElement_Bomb : public CBasicEffect
{
private:
	explicit CElement_Bomb();
	virtual ~CElement_Bomb() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTarget);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;
private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	CTransform*		m_pTargetTrans = nullptr;
	CTransform*		m_pChildren	   = nullptr;

private:
	_double			m_Duration = 3.0;
	_double			m_DurationDelta = 0;
	_float			m_fScale = 1.f;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* _pTarget);
	virtual void Free();
};

END