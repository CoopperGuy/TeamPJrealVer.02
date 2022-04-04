#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectPhase2Aura final :
	public CBasicEffect
{
private:
	explicit CEffectPhase2Aura();
	virtual ~CEffectPhase2Aura() = default;
public:
	virtual HRESULT Initialize(void* pArg, CTransform* pTransform);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectPhase2Aura* Create(void* pTarget, CTransform* pTransform);
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