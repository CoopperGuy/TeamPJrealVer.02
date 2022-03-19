#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectMeteoFire final :
	public CBasicEffect
{
private:
	explicit CEffectMeteoFire();
	virtual ~CEffectMeteoFire() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectMeteoFire* Create(void * pArg);
	virtual void Free() override;


private:

};

END