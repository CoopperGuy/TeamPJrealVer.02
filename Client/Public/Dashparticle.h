#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CDashparticle : public CBasicEffect
{
private:
	explicit CDashparticle();
	virtual ~CDashparticle() = default;

private:
	virtual HRESULT Initialize(CEmptyEffect* pThis);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;

private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CVIBuffer_RectInstance* m_pParticle = nullptr;

private:
	_float			m_DurationDelta = 0;
	_float			m_fScale = 0.f;
	_float3			m_vScale;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis);
	virtual void Free();
};

END