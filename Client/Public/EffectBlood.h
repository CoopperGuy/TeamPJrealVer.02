#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectBlood final :
	public CBasicEffect
{
private:
	explicit CEffectBlood();
	virtual ~CEffectBlood() = default;
public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectBlood* Create(void*	pTarget, _vector pos);
	virtual void Free() override;

private:
	_double m_dDeadTime = 0;

	_float3 Startscail = {};
	_float ScailX = 0.f;
	_float ScailY = 0.f;

	_float PosX = 0.f;
	_float PosY = 0.f;

	int RandomNum = 0;
};

END