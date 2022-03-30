#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectRockDecal final :
	public CBasicEffect
{
private:
	explicit CEffectRockDecal();
	virtual ~CEffectRockDecal() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectRockDecal* Create(void * pArg, _vector pos);
	virtual void Free() override; 

private:
	_double m_dDeadTime = 0;
	_double m_makefb = 0;
	_double m_makedt = 0;

	_bool makemeteo = false;
	
	_vector mypos = {};

};

END