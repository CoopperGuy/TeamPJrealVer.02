#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CFootHammer_Decal : public CBasicEffect
{
private:
	explicit CFootHammer_Decal();
	virtual ~CFootHammer_Decal() = default;

private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTarget, _uint iCount,_fvector vDist);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;

private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;

private:
	_float			m_DurationDelta = 0;
	_float			m_fAngle		= 0.f;
	_bool			m_bCreateEffect = true;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* _pTarget, _uint iCount, _fvector vDist);
	virtual void Free();
};

END