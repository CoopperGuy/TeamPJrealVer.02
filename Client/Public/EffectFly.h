#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectFly final :
	public CBasicEffect
{
private:
	explicit CEffectFly();
	virtual ~CEffectFly() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectFly* Create(void*	pTarget);
	virtual void Free() override;


public:
	void SetGameObject(CGameObject * obj);
	void SetTransform(CTransform * trans);

	void SetFadeInOut(CGameObject * pObj);


private:
	_double DeadTime = 0.f;

	_bool	m_bisRealDead = false;
	_bool	m_bSetFadeInOut = true;
};

END