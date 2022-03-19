#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectMeteoTrail final :
	public CBasicEffect
{
private:
	explicit CEffectMeteoTrail();
	virtual ~CEffectMeteoTrail() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectMeteoTrail* Create(void * pArg);
	virtual void Free() override;

	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);

private:
	_double	m_dFadeOutDuration = 0;
};

END