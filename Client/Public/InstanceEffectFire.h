#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CInstanceEffectFire final :
	public CBasicEffect
{
private:
	explicit CInstanceEffectFire();
	virtual ~CInstanceEffectFire() = default;
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
	static CInstanceEffectFire* Create(void* pTarget = nullptr);
	virtual void Free() override;


public:
	void SetGameObject(CGameObject * obj);
	void SetTransform(CTransform * trans);
	void SetFadeInOut(CGameObject* pObj);

private:
	_double m_DeadTimeAcc = 0.0;

	_double FadeInStartTime = 0.f;

	_bool	m_bisEnd = false;

	_vector TargetPos = {};

	_float3  m_fScail = {};

};

END