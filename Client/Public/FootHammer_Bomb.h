#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CFootHammer_Bomb : public CBasicEffect
{
private:
	explicit CFootHammer_Bomb();
	virtual ~CFootHammer_Bomb() = default;

private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTarget);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;

private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	class CObb*		m_pOBB		   = nullptr;

private:
	_float			m_DurationDelta = 0;
	_float			m_fScale = 0.f;
	_float3			m_vScale;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* _pTarget);
	virtual void Free();
};

END