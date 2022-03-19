#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectMagic final :
	public CBasicEffect
{
private:
	explicit CEffectMagic();
	virtual ~CEffectMagic() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectMagic* Create(void * pArg, _vector pos);
	virtual void Free() override; 

private:
	_double m_dDeadTime = 0;
};

END