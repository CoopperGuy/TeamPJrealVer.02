#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectRockDust final :
	public CBasicEffect
{
private:
	explicit CEffectRockDust();
	virtual ~CEffectRockDust() = default;
public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectRockDust* Create(void*	pTarget);
	virtual void Free() override;

private:
	CComponent* m_pComponent = nullptr;

	_double m_dDeadTime = 0;

	_float3 Startscail = {};
	_float ScailX = 0.f;
	_float ScailY = 0.f;

	_float PosX = 0.f;
	_float PosY = 0.f;

	int RandomNum = 0;
};

END