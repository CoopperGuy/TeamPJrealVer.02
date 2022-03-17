#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectFlyLaser final :
	public CBasicEffect
{
private:
	explicit CEffectFlyLaser();
	virtual ~CEffectFlyLaser() = default;
public:
	void SetIsEnd() { m_bisEnd = true; }
public:
	_bool GetIsEnd() { return m_bisEnd; }
public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectFlyLaser* Create(void*	pTarget);
	virtual void Free() override;


public:
	void SetGameObject(CGameObject * obj);
	void SetTransform(CTransform * trans);
	void SetFadeInOut(CGameObject* pObj);

private:
	_double FadeInStartTime = 0.f;

	_bool	m_bisEnd = false;

	_vector TargetPos = {};

	_float3  m_fScale = {};

};

END