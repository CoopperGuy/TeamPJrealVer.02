#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectSmoke final :
	public CBasicEffect
{
private:
	explicit CEffectSmoke();
	virtual ~CEffectSmoke() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectSmoke* Create(void * pArg, _vector pos);
	virtual void Free() override;


private:

};

END