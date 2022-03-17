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
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectBlood* Create(void*	pTarget);
	virtual void Free() override;
};

END