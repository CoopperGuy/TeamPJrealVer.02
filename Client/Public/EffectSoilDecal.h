#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectSoilDecal final :
	public CBasicEffect
{
private:
	explicit CEffectSoilDecal();
	virtual ~CEffectSoilDecal() = default;
public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectSoilDecal* Create(void*	pTarget, _vector pos);
	virtual void Free() override;

private:
	_double m_dDeadTime = 0;

	_float3 Startscail = {};
	_float ScailX = 0.f;
	_float ScailY = 0.f;
};

END