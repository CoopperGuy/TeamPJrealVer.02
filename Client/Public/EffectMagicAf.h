#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectMagicAf final :
	public CBasicEffect
{
private:
	explicit CEffectMagicAf();
	virtual ~CEffectMagicAf() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectMagicAf* Create(void * pArg, _vector pos);
	virtual void Free() override;


private:
	_double m_dFadeoutDu = 0;


};

END