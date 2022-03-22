#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CSlashWave :
	public CBasicEffect
{
private:
	explicit CSlashWave();
	virtual ~CSlashWave() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTarget);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;
private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
private:
	CTransform*		m_pTargetTrans = nullptr;
	CTransform*		m_pChildren	   = nullptr;
private:
	_double			m_Duration = 3.0;
	_double			m_DurationDelta = 0;
	_float			m_fScale = 1.f;
	_float3			m_vecScales;
private:
	CObb*	m_pOBB = nullptr;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* _pTarget);
	virtual void Free();
};

END