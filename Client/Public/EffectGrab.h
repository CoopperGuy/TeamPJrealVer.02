#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectGrab final :
	public CBasicEffect
{
private:
	explicit CEffectGrab();
	virtual ~CEffectGrab() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectGrab* Create(void*	pTarget);
	virtual void Free() override;
	
	
	void SetFadeInOut(CGameObject * pObj);

public:
	void SetSlashR(_bool ptrue) { m_bSlashR = ptrue; }
public:
	CGameObject* pTargetObj = nullptr;
private:
	_bool	m_bSetFadeOut= true;
	_bool m_bSlashR = false;

	_vector TargetPos = {};
	_vector StartScale = {};


	_float3  m_fScale = {};
	_float3 m_fScrollX = {};
	_float3 m_fScrollY = {};



	_float plusscale = 0.01f;
	_float m_fMaxScail = 0.3f;

};

END