#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectFireTwist final :
	public CBasicEffect
{
private:
	explicit CEffectFireTwist();
	virtual ~CEffectFireTwist() = default;
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
	static CEffectFireTwist* Create(void*	pTarget);
	virtual void Free() override;


public:
	void SetGameObject(CGameObject * obj);
	void SetTransform(CTransform * trans);

private:
	CGameObject* pTargetObj = nullptr;

private:
	_double FadeInStartTime = 0.f;

	_bool	m_bisEnd = false;

	_vector TargetPos = {};
	_vector StartScail = {};


	_float3  m_fScail = {};

	_float plusscail = 1.f;


};

END