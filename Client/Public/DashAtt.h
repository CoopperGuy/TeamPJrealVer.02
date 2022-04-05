#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CDashAtt : public CBasicEffect
{
private:
	explicit CDashAtt();
	virtual ~CDashAtt() = default;

private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTarget);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;

private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	CTransform*		m_pTargetTransform = nullptr;
	class CObb*		m_pOBB		   = nullptr;

private:
	_float			m_DurationDelta = 0;
	_float			m_fScale = 0.f;
	_float3			m_vScale;
	_float3			m_vLook;
	_bool			m_bCreate = true;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* _pTarget);
	virtual void Free();
};

END