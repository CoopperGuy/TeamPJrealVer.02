#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectlaser final :
	public CBasicEffect
{
private:
	explicit CEffectlaser();
	virtual ~CEffectlaser() = default;
public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectlaser* Create(void*	pTarget);
	virtual void Free() override;
};

END