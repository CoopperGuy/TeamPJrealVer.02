#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectVine final :
	public CBasicEffect
{
private:
	explicit CEffectVine();
	virtual ~CEffectVine() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectVine* Create(void*	pTarget);
	virtual void Free() override;


private:
	_double DeadDt = 0;



};

END