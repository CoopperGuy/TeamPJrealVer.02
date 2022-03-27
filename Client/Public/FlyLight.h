#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CFlyLight final :
	public CBasicEffect
{
private:
	explicit CFlyLight();
	virtual ~CFlyLight() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CFlyLight* Create(void*	pTarget);
	virtual void Free() override;

private:
	_double DeadTime = 0.f;

	_bool	m_bisRealDead = false;
	_bool	m_bSetFadeInOut = true;
};

END