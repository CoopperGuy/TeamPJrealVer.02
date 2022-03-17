#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectSwordRing final :
	public CBasicEffect
{
private:
	explicit CEffectSwordRing();
	virtual ~CEffectSwordRing() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectSwordRing* Create(void*	pTarget);
	virtual void Free() override;
	
	
	void SetFadeInOut(CGameObject * pObj);


public:
	void SetSlashR(_bool ptrue) { m_bSlashR = ptrue; }
private:
	_bool	m_bSetFadeOut= true;
	_bool m_bSlashR = false;

	_vector TargetPos = {};
	_vector StartScale = {};


	_float3  m_fScale = {};
	_float3 m_fScrollX = {};
	_float3 m_fScrollY = {};


	_float PlusScale = 1.f;
	_float plusscale = 1.f;
	_float m_fMaxScail = 2.f;
	_float m_fDeadTime = 0.f;

private:
	CObb*	m_pOBB = nullptr;

};

END