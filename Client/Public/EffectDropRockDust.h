#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectDropRockDust final :
	public CBasicEffect
{
private:
	explicit CEffectDropRockDust();
	virtual ~CEffectDropRockDust() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectDropRockDust* Create(void*	pTarget, _vector pos);
	virtual void Free() override;



private:
	_double FadeInStartTime = 0.f;

	_bool	m_bSet = false;

	_vector TargetPos = {};

	_float3  m_fScail = {};
private:
	_float m_fFrame = 0;


};

END