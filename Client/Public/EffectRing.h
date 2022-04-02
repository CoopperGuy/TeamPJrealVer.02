#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectRing final :
	public CBasicEffect
{
private:
	explicit CEffectRing();
	virtual ~CEffectRing() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectRing* Create(void*	pTarget, _vector pos);
	virtual void Free() override;
	
	

private:
	_double m_dDeadTime = 0;
	_float3 StartScail = {};

private:
	CObb*	m_pOBB = nullptr;

};

END