#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectDust final :
	public CBasicEffect
{
private:
	explicit CEffectDust();
	virtual ~CEffectDust() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectDust* Create(void*	pTarget);
	virtual void Free() override;

public:
	void SetGameObject(CGameObject * obj);
	void SetTransform(CTransform * trans);
	void SetFadeInOut(CGameObject* pObj);

private:
	_double FadeInStartTime = 0.f;

	_bool	m_bSet = false;

	_vector TargetPos = {};

	_float3  m_fScail = {};



};

END