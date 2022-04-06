#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CItemDropEffect final :
	public CBasicEffect
{
private:
	explicit CItemDropEffect();
	virtual ~CItemDropEffect() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CItemDropEffect* Create(void*	pTarget, _vector pos);
	virtual void Free() override;

private:
	_double m_dDeadTime = 0.0;

};

END